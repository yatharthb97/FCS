//box.hpp
#pragma once


// Libraries
#include <fstream>
#include<sstream>
#include <cmath>
#include <random>
#include<iomanip>
#include<string>
#include<vector>
#include<cstdint>
#include <iostream>
#include <stdlib.h>

#include "macros.hpp"
#include "particle.hpp"
#include "gl.hpp"
#include "classes.hpp"

#include "./submodules/vector.hpp"
#include "./submodules/random.hpp"
//#include "./submodules/filesystem.hpp"
//#include "./submodules/timer.hpp"
#include "./submodules/json/single_include/nlohmann/json.hpp"   //Json Library Used



class LangevinBox
{

private:
	//private declarations



public:

// ------------------------   Member Variables   ---------------------------------

  //1. Set Box Configuration
  int BoxID = 0;//-------------------------------> Unique ID Assigned to the Box
  //static int dim = 3;//-------------------------> dimensions of system


  //2. Extern Objects
  Laser laser;
  Veff veff;
  SimClock simclock; //TODO → Remove simclock, dt, maxsteps redundancy

  //3. Box Configuration
  double Edge;//-------------------------> Edge of the box
  double Rho;//--------------------------> Number Density
  unsigned long int Part_no;//-----------> Number of particles in the box
  unsigned long int T_stepsMax;//--------> Total Number of Steps Performed
  unsigned long int FrameExports;//------> Number of Frames to be saved (distributed Uniformly)
  double dt;//---------------------------> Smallest Incremented Timestep
  double MSD = 0;//----------------------> MSD Value for the entire evolution
  unsigned long int SimCounter = 0;//----> Simulation Counter


  //4. Vector of Particles
  std::vector<Particle> partlist; //Frequently refereed to as partlist_t partlist

  //5. Observation Volume
  V AD_Radius;//--------------> Bounds for Veff
  V AD_PSF;//-----------------> Exponents for PSF - precalculated
  double PSF_Norm = 1.0;//----------> Normalization for PSF function

  //6. PRNG Resources
  MT_RND rnd; //MT PRNG
  std::uniform_real_distribution<double> u_dist; // Uniform dist
  std::normal_distribution<double> gauss_dist; // Normal dist. for Langevin

  //7. Datapipe
  Datapipe datapipe;

  //8. String Resources
  std::string box_path;
  std::string profile_str;


// ------------------------   Member Functions  ---------------------------------


  	//1. Trivial Constructor (deleated explicitly)
  	LangevinBox() = delete;

  	//2. Default Constructor
  	LangevinBox(double Rho, 
  				unsigned int Part_no, 
  				int FrameExports, 
  				const SimClock &simclock, 
  				const Laser &laser,
  				const Veff &veff): 
  				Rho(Rho), 
  				Part_no(Part_no),
  				FrameExports(FrameExports),
 				simclock(simclock),
  				laser(laser), 
  				veff(veff)
  	{

      //Edge and parameter setting
      this->Edge = std::cbrt(Part_no/Rho);
      this->T_stepsMax = simclock.MaxSteps;
      this->dt = simclock.StepSize;

      //Call Box Init
  	  this->Init();

  	}

  	//3.0 Initilization function
  	void Init()
  	{
  	
  	  //3.0 Check if global NewSession was called.
  	  if(!gl::session_declared)
  	  {
  	  	std::cerr << "[FATAL ERROR] NewSession() was not called before of LangevinBox().\n";
  	  	exit(-1);
  	  }

      
      //3.1 BoxPath Creation ////// TODO → Mutax Protect
  	  gl::boxcounter++;
  	  this->BoxID = gl::boxcounter; //Set BoxID


  	  //Assumes → gl::session_path is guarenteed to be a valid path.

  	 if(this->BoxID == 0 && gl::boxmode == "singlebox")
  	  {
  	  	//No Subfolder is created
  	  	this->box_path = gl::session_path; 
  	  }

  	  else if(this->BoxID > 1 && gl::boxmode == "singlebox")
  	  {
  	  	//Invalid State
  	  	std::cerr << "[ERROR] New box cannot be created in \"singlebox\" mode.\n Set up session with \"multibox\" mode.\n";
  	  	exit(-1);
  	  }


  	  else if(gl::boxmode == "multibox")
  	  {
  	  	//Create Subfolder
  	  	std::string path = gl::session_path;
  	  	
  	  	path.append(std::to_string(BoxID));
  	  	FileSystem::SlashIt(path);
  	  	this->box_path = path;

  	  	//Make Subfolder
  	  	mkdir(path.c_str(), 0777);
  	  }

  	  else
  	  	std::cerr << "[FATAL ERROR] Invalid State:" << __LINE__ << __FILE__ << '\n';
  	//---------------


      //3.2.1 Prepare RND Generator
  	  rnd.NewSeeds(); //Seed PRNG
  	  rnd.Discard();  //Warm Up Generator

  	  std::string seedpath = box_path;
  	  seedpath.append("seeds");
  	  seedpath.append(FCS_DATAFILE_EXT);

  	  rnd.SaveSeed(seedpath); //Save Seeds for the simulation
 

      //3.2.2 Reset the distribution parameters
      u_dist.param(std::uniform_real_distribution<double>::param_type(0.0, 1.0));
      gauss_dist.param(std::normal_distribution<double>::param_type(0.0, 1.0));

      //3.3 Set up Observation Volume → Given Radius and Structure Factor
  	#if FCS_VEFF_GAUSSIAN == 1 //Block that defined a *3D Gaussian* Veff

  		//Equivalent to {2/(rx^2), 2/(ry^2), 2/(rz^2)} in the PSF Expression
  		AD_PSF.x = 2/(veff.radius*veff.radius);
  		AD_PSF.y = 2/(veff.radius*veff.radius);
  		AD_PSF.z = 2/(veff.radius*veff.radius*veff.sf*veff.sf);

 		AD_Radius.x =  veff.radius;	
 		AD_Radius.y =  veff.radius;
 		AD_Radius.z =  veff.radius * veff.sf;

 		veff.vol_gauss(); // Set appropriate volume → Ignored Return Value

  	#elif FCS_VEFF_GAUSSIAN == 0 //Block that defines a *Spherical* Veff
  		AD_PSF.setscalar(veff.radius); //Treat as scalar
  		AD_Radius = V(veff.radius, veff.radius, veff.radius);

  		veff.vol_sphere(); // Set appropriate volume → Ignored Return Value
  	#endif

  		//3.4 Particle Init
  		partlist.reserve(Part_no);
  		for(int i = 0; i < Part_no; i++)
  		{
  				//Particle(unsigned int partid, V &pos)
  		  	V posx(rndPosition(), rndPosition(), rndPosition());
  		  	partlist.emplace_back(i, posx);
  		}

	    //3.5 Print Box Profile
	    this->profile_str = this->Profile();
	    std::cout << "\n\n< New Box Created >" << '\n';

	    std::cout << this->profile_str << std::flush;
    ////////////////////////////////////////////////

      





  	//3.6. Write Config
  	JsonConfigWrite(box_path);



    //3.7. Write 0th position frame
    WritePosFrame(partlist, 0);


  	} //End of Init()


  	//4. Position Initalization Function
  	double rndPosition() //For Initalization
  	{
  		#if FCS_SYMMETRIC_BOX == 1
  			return Edge*(0.5 - u_dist(rnd.engine)); //Symmetric Box
  		#elif FCS_SYMMETRIC_BOX == 0
  			return Edge*u_dist(rnd.engine); //Assymetric Box
  		#endif

  	} //End of rndPosition()

  	//5. PBC → Periodic Boundary Conditions Function
    void PBC(V &pos)
    {
      
      #if FCS_SYMMETRIC_BOX == 1  //Symmetric Box
        pos.x -= Edge*rint(pos.x/Edge);
        pos.y -= Edge*rint(pos.y/Edge);
        pos.z -= Edge*rint(pos.z/Edge);
      #elif FCS_SYMMETRIC_BOX == 0 //Assymetric Box
        pos.x = (pos.x > Edge)*(pos.x - Edge) + (pos.x < 0)*(Edge + pos.x);
        pos.y = (pos.y > Edge)*(pos.y - Edge) + (pos.y < 0)*(Edge + pos.y);
        pos.z = (pos.z > Edge)*(pos.z - Edge) + (pos.z < 0)*(Edge + pos.z);
      #endif

    } //End of PBC()



    //6. Evolve Function → Simulation Function
    void Evolve() //Do Evolution of Box
    {
      
      //6.1 Calculate loop partitions
      const unsigned long int outerloop =  (FrameExports > 0)*FrameExports + (FrameExports == 0)*1;
      const unsigned long int innerloop = T_stepsMax/outerloop;

      //6.2 Outer Loop → For FrameExports
      for(unsigned long int j = 0; j < outerloop; j++) //Outer Loop
      {

          //6.3 Inner Loop → Unit Loop 
          for(unsigned long int i = 0; i < innerloop; i++) //Inner Loop
          {
            //For One Time Step ↓        

            //6.3.0
            SimCounter++; //Increment Simulation SimCounter

            //6.3.1 Move All particles
            //<x^2> = 2*dim*D*dt = 4*D*t
            for(auto &part : this->partlist)   //sqrt(4*i.D*dt)*gauss_dist(mt);
            {
              part.pos.x += std::sqrt(2*part.D*dt)*gauss_dist(rnd.engine);
              part.pos.y += std::sqrt(2*part.D*dt)*gauss_dist(rnd.engine);
              part.pos.z += std::sqrt(2*part.D*dt)*gauss_dist(rnd.engine);

              PBC(part.pos);
            } //End of minor loop

            //6.3.2
            //Count the number of particles in the observation Volume & MSD Calculation
            unsigned int InVolCount = 0; //Counts the number of particles in the Veff Volume
            unsigned int FlashCount = 0; //Counts the number of partilces that flashed in the instant
            double MSDi = 0; //Instantaneous MSD
            for(auto &part : partlist) 
            {
              
              //...1 MSD Calc
              V msd_vec = part.pos - part.init_pos;
              PBC(msd_vec); //TODO → Ask if this is strictly necessary
              MSDi += msd_vec.size_sq();

              //...2 Check if the particle is in the observation Volume (2 different if statements)
            #if FCS_VEFF_GAUSSIAN == 0 //Spherical Assumption → Assume Sphere of dia AD_Radius.X()
              if(part.pos.size_sq() <= AD_Radius.size_sq()) 
            #elif FCS_VEFF_GAUSSIAN == 1 //Assume 3D Gausssian Observation Volume
              if(part.pos.X() <= AD_Radius.X() &&
              	 part.pos.Y() <= AD_Radius.Y() &&
              	 part.pos.Z() <= AD_Radius.Z())
            #endif //FCS_VEFF_GAUSSIAN
              {
                  //Set InVol flag TODO -> Test Necessacity
                  part.InVol = true;

                  //Particle is in Volume
                  InVolCount++;
                   

                  //...3 Try flashing with set probability
                  if(u_dist(rnd.engine) < PulseProbablity(part)) 
                  {
                      part.last_flash = SimCounter;
                      part.Flash(); //Automatically sets "bleached state" if condition is met
                      FlashCount++; //Particle did flash
                  }
              }

              else
                part.InVol = false; //TODO -> Test Necessacity
            } //End of minor loop

            ////6.3.3
            //Normalizations & DataPipeIn
            MSD += (MSDi)/double(Part_no); //Total MSD count per particle

            datapipe.stats << double(SimCounter)*dt << FCS_DSep << MSD/double(SimCounter) << FCS_DSep
                          	  //Elapsed Time         			   //Normed MSD

                           << InVolCount << FCS_DSep << FlashCount << '\n';
                          	//Particles InVol            //Particles that Flashed

          } //End of Inner Evolution Loop


          ////6.3.4 FrameExport
          WritePosFrame(partlist, j+1);

      } //End of Outer Evolution Loop

    
    // END of SIMULATION LOOP - Out Calculations ↓ [--------] 

      //Flush Data Pipes
      datapipe.Flush(box_path);

      //Run Python Script
      if(gl::run_python)
      {
      	//Launch python script TODO --> use a cleaner approach
      	std::ostringstream command;
      	command << "python3  Plots.py" << "  " //-----> Script Name
      			<< this->box_path << "  " //----------> Box Path
      	        << "C++";  //-------------------------> Operation Mode
      	system(command.str().c_str());
      }

    } //End of Evolve()



    double PulseProbablity(const Particle &part) const
    {
      //Probablity is the product of all the seperate probablities
	    #if FCS_VEFF_GAUSSIAN == 1 //Gaussian
	      return part.qm_yield * PSF(part.pos) * laser.prob(SimCounter);
	    #elif FCS_VEFF_GAUSSIAN == 0 //Spherical
	      return part.qm_yield * laser.prob(SimCounter);
	    #endif

    }


    //Will not work with Asymmetric Box
    double PSF(const V &pos) const
    {
      //AD_Sigma = Airy Disk Sigma (x=y, z=sf*x)
      double exponent = (pos.X_sq()*AD_PSF.X_sq()*-1 + 
                         pos.Y_sq()*AD_PSF.Y_sq()*-1 +
                         pos.Z_sq()*AD_PSF.Z_sq()*-1);

      return PSF_Norm * std::exp(exponent);
  
    } //End of PSF()





    //Saves the dat file of the box snapshot
    void WritePosFrame(partlist_t &partlist_t, unsigned long int frameid)
    {
      std::ostringstream buffer;
      buffer << std::setprecision(FCS_FLOAT_PRECISION);


      unsigned int sizex = partlist.size();
      for(unsigned int i = 0; i < sizex; i++) 
      {
        buffer /*<< i << FCS_DSep*/ << partlist[i].pos.info(FCS_DSep) << FCS_DSep
              //Partid                 //Position {x,y,z}
               << partlist[i].InVol << FCS_DSep << (partlist[i].last_flash == SimCounter) << '\n';
                  //InVol (bool)                   //Flash? (bool)
      }


      std::string filename = box_path;
      filename.append(std::to_string(frameid));
      filename.append(FCS_DATAFILE_EXT);

      std::ofstream file(filename, std::ios::out); //Write to file
      file << buffer.str();  //Open File
      file.close();  //Close File

    } // End of FrameExport()




    void inline JsonConfigWrite(const std::string &parentpath)
    {
    	
    	// for convenience
    	using json = nlohmann::json;

    	json config; //Init Json Object

    	config["Path"] =  this->box_path;//---------------->1
    	config["Edge"] = this->Edge;//--------------------->2
    	config["FrameExports"] = this->FrameExports;//----->3
    	config["Radius"] = this->veff.radius;//------------>4
    	config["Part_no"] = this->Part_no;//--------------->5
    	config["T_stepsMax"] = this->T_stepsMax;
    	config["dt"] = this->dt;


    	config["run_python"] = gl::run_python; //Specifies History → Unused [[unused]]
    	config["do_pos_plots"] = gl::do_pos_plots ;
    	config["show_py_plots"] = gl::show_py_plots;

    	//Note that char types are not automatically converted to JSON strings, but to integer numbers. A conversion to a string must be specified explicitly (source: library docs)
    	config["D_Sep"] = std::string(1, FCS_DSep);//---------------------->6

    	
    	//Open(Create) File
    	std::string filename = parentpath;
    	filename.append("config.json");
    	std::ofstream file(filename, std::ios::out);
    	
    	file << std::setw(4) << config; //Write to file with intendation ws = 4
    	file.close(); //Close file
    } //End of JsonConfigWrite()


    std::string Profile() const
    {
    	
    	std::ostringstream buffer;
    	buffer << std::setprecision(FCS_FLOAT_SHORT_PRECISION) << std::boolalpha;
    	buffer << " • BoxID: " << this->BoxID << " | Box Path: " << this->box_path << '\n';
    	buffer << " • First Seed used for MT PRNG: " << rnd.FirstSeed() << '\n';

    	buffer << "\n< Box Statistics >\n"; //BOX
    	buffer << " • Box Edge: "<< Edge << " | No. Density: " << Rho << " | Particles: " << Part_no;
    	buffer << "\n • Total Steps: " << T_stepsMax << " | Step Size: " << dt << '\n';
    	buffer << " • Position Frames Normalizations: " << FrameExports << "/" << T_stepsMax << '\n';

    	//Declare if Symmetric Box and if Gaussian PSF
    	buffer << " • Box Symmetric: " << (FCS_SYMMETRIC_BOX == 1) << " | PSF Type: "
    		   << (FCS_VEFF_GAUSSIAN == 1 ? "3D Gaussian" : "Uniform Spherical") << '\n';

    	buffer << "\n< Veff Statistics >\n"; //VEFF
    	buffer << " • xy-Radius: " << veff.radius << " | Structure Factor: " << veff.sf << "\n";
    	buffer << " • Volume of Veff: " << veff.vol << " | Bound Radius: " << AD_Radius << '\n';
    	buffer << " • PSF Exponents: " << AD_PSF << " | PSF Normalization: " << PSF_Norm << '\n';
    	
    	buffer << "\n< Laser Statistics >\n"; //LASER
    	buffer << " • Pulsing Freq: " << laser.PulseFrequency << " | Char Decay Time: " <<  laser.CharDecayTime << '\n';

    	//---
    	buffer << "\n  -END OF PROFILE-\n";


    	return buffer.str();

    } //End of profile()


}; //End of class LangevinBox[]



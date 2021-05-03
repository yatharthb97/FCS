//classes.hpp
#pragma once

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
#include "./submodules/vector.hpp"
#include "./submodules/random.hpp"
#include "./submodules/filesystem.hpp"
//#include "./submodules/timer.hpp"

//Type declarations
//using partlist_t = std::vector<Particle>;
//using unsigned long int = std::uint64_t;
//using  enum_int_t = int;


/*class OBVol //Observation Volume
{
public:
  double radius;
  double radius_sq;

  //Default Constructor
  OBVol(const double radius): radius(radius), radius_sq(radius*radius)
  {}
}; //End of class OBVol*/

class Datapipe
{
    
public:

    std::ostringstream stats;

    Datapipe()
    {
      stats << std::setprecision(FCS_FLOAT_PRECISION);
    }

    bool inline Flush(const std::string &parentpath)
    {
      bool errnox = false;
      if(!parentpath.empty())
      {
        errnox = WriteToFile(std::string(parentpath + "stats.dat"), stats.str());
      }
      return errnox;
    }


private:
  
  bool inline WriteToFile(std::string path, const std::string &content)
  {
    std::ofstream file(path, std::ios::out);
    if(file.is_open())
    { 
      file << content;
      return true;
      //RAII close expected.
    }
    else
      return false;

    //RAII file close expected
  }

}; //End of class Datapipe

class LangevinBox
{
public:

	//Member Functions
	int BoxID = 0;


	double Edge;          //Edge of the box
	double Rho;           //Number Density
	unsigned long int Part_no;      //Number of particles in the box
  unsigned long int T_stepsMax;   //Total Number of Steps Performed
  unsigned long int FrameExports; //Number of Frames to be saved
  double dt;            //Smallest Incremented Timestep
  double MSD = 0;       //MSD Value for the entire evolution
  unsigned long int SimCounter = 0;
  int dim = 3;
  //Vector of Particles
  std::vector<Particle> partlist;

  //Observation Volume
  //OBVol Obvol(1.0); //Radius is 1.0

  //PRNG Resources
	MT_RND rnd; //MT PRNG
	std::uniform_real_distribution<double> u_dist; // Uniform dist
  std::normal_distribution<double> gauss_dist; // Normal dist. for Langevin

  //Datapipe
  Datapipe datapipe;

  //Paths
  std::string box_path;
  std::string session_name; //TODO


  	LangevinBox(double Rho, unsigned int Part_no, int FrameExports, std::string session_name): Rho(Rho), Part_no(Part_no), FrameExports(FrameExports), session_name(session_name)
  	{
  		
      //Edge and parameter setting
      Edge = std::cbrt(Part_no/Rho);
      T_stepsMax = 1e5;
      dt = 1e-3;
      

      //Call Box Init
  		this->Init();

  	}

  	void Init()
  	{
  		
      //BoxPath Creation
      using namespace FileSystem;
      std::string path = FCS_GPARENT_PATH;
      //SlashIt(path);
      //path.append(session_name);
      //SlashIt(path);
      //path.append(std::tostring(BoxID));

      box_path = MakePrimaryNode(path, session_name); //Reserve Directory
      std::cout << box_path << std::endl;



      //1. Prepare RND Generator
  		rnd.NewSeeds(); //Seed PRNG
  		rnd.Discard();  //Warm Up Generator

      //Reset the distribution parameters
      u_dist.param(std::uniform_real_distribution<double>::param_type(0.0, 1.0));
      gauss_dist.param(std::normal_distribution<double>::param_type(0.0, 1.0));

  		//2. Particle Init
  		partlist.reserve(Part_no);

  		for(int i = 0; i < Part_no; i++)
  		{
  			//Particle(unsigned int partid, V &pos)
        V posx(rndPosition(), rndPosition(), rndPosition());
        partlist.emplace_back(i, posx);
  		}

      //Write 0th frame
      WriteFrame(partlist, 0);


  	} //End of Init()


  	double rndPosition()
  	{
  		#if FCS_SYMMETRIC_BOX == 1
  			return Edge*(0.5 - u_dist(rnd.engine)); //Symmetric Box
  		#elif FCS_SYMMETRIC_BOX == 0
  			return Edge*u_dist(rnd.engine); //Assymetric Box
  		#endif

  	} //End of rndPosition()


    void PBC(V &pos)
    {
      
      #if FCS_SYMMETRIC_BOX == 1
        pos.x -= Edge*rint(pos.x/Edge);
        pos.y -= Edge*rint(pos.y/Edge);
        pos.z -= Edge*rint(pos.z/Edge);
      #elif FCS_SYMMETRIC_BOX == 0
        pos.x = (pos.x > Edge)*(pos.x - Edge) + (pos.x < 0)*(Edge + pos.x);
        pos.y = (pos.y > Edge)*(pos.y - Edge) + (pos.y < 0)*(Edge + pos.y);
        pos.z = (pos.z > Edge)*(pos.z - Edge) + (pos.z < 0)*(Edge + pos.z);
      #endif

    } //End of PBC()




    void Evolve() //Do Evolution of Box
    {
      
      const unsigned long int outerloop =  (FrameExports > 0)*FrameExports + (FrameExports == 0)*1;
      const unsigned long int innerloop = T_stepsMax/outerloop;

      for(unsigned long int j = 0; j < outerloop; j++) //Outer Loop
      {

          for(unsigned long int i = 0; i < innerloop; i++) //Inner Loop
          {
            //For One Time Step ↓        

            //0
            SimCounter++; //Increment Simulation SimCounter

            //1 <x^2> = 2*d*D*t = 4*D*t
            //Move All particles
            for(auto &part : partlist)   //sqrt(4*i.D*dt)*gauss_dist(mt);
            {
              part.pos.x += std::sqrt(2*dim*part.D*dt)*gauss_dist(rnd.engine); //4*D*time_step*WhiteNoise
              part.pos.y += std::sqrt(2*dim*part.D*dt)*gauss_dist(rnd.engine);
              part.pos.y += std::sqrt(2*dim*part.D*dt)*gauss_dist(rnd.engine);

              //PBC(part.pos);
            }

            //2
            //Count the number of particles in the observation Volume & MSD Calculation
            unsigned int InVolCount = 0; //Counts the number of particles in the OB Volume
            unsigned int FlashCount = 0; //Counts the number of partilces that flashed in the instant
            double MSDi = 0;
            for(auto &part : partlist) 
            {
              
              //MSD Calc
              V msd_vec = part.pos - part.init_pos;
              //PBC(msd_vec);
              MSDi += msd_vec.size_sq();

              //Check if the particle is in the observation Volume
              if(part.pos.size_sq() <= 1.0) 
              {
                  //Set InVol flag
                  part.InVol = true;
                  InVolCount++;
                    
                  // Try flashing with set probability
                  if(u_dist(rnd.engine) < part.e_prob) 
                  {
                      part.last_flash = i;
                      FlashCount++;
                  }
              }

              else
                part.InVol = false;
            }

            //3
            //Normalizations & DataPipeIn
            MSD += (MSDi)/double(Part_no); //Total MSD count per particle
            datapipe.stats << double(SimCounter)*dt << FCS_DSep << MSD/double(SimCounter)/* << FCS_DSep 
                          //Elapsed Time         //Normed MSD

                           << InVolCount << FCS_DSep << FlashCount */<< '\n';
                          //Particles InVol            //Particles that Flashed

          } //End of Inner Evolution Loop


          //One FrameExport
          WriteFrame(partlist, j+1);

      } //End of Outer Evolution Loop

      

      datapipe.Flush(box_path);

      // parent_path = sys.argv[1]
      // edge = float(sys.argv[2])
      // frames = int(sys.argv[3])
      // ob_radius = float(sys.argv[4])
      //part_nums = int(sys.argv[5])


      std::ostringstream command;
      command << "python3  Plots.py  \"" << box_path << "\"  " << Edge << "  " << FrameExports << "  " << 1.00 << "  " << Part_no;  
      system(command.str().c_str());

    } //End of Evolve()



    //Saves the dat file of the box snapshot
    void WriteFrame(std::vector<Particle> &partlist_t, unsigned long int frameid)
    {
      std::ostringstream buffer;
      buffer << std::setprecision(FCS_FLOAT_PRECISION);


      unsigned int sizex = partlist.size();
      for(unsigned int i = 0; i < sizex; i++) 
      {
        buffer /*<< i << FCS_DSep*/ << partlist[i].pos.info(FCS_DSep) << FCS_DSep
              //Partid             //Position
               << partlist[i].InVol << '\n';
                  //InVol (bool)
      }


      std::string filename = box_path;
      FileSystem::SlashIt(box_path);
      filename.append(std::to_string(frameid));
      filename.append(".dat");

      std::ofstream file(filename, std::ios::out); //Write to file
      file << buffer.str();  //Open File
      file.close();  //Close File
    } // End of FrameExport()







}; //End of class LangevinBox
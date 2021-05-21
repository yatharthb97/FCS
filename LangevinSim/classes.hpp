//classes.hpp
#pragma once

#include <fstream>
#include<sstream>
#include<iomanip>
#include <iostream>
#include <cmath>
#include <vector>
#include "macros.hpp"

#include "./submodules/filesystem.hpp"
//#include "./submodules/timer.hpp"



class Laser
{
public:

  ulint_t CharDecayTime;
  ulint_t PulseFrequency;

  //Constructor
  Laser(ulint_t PulseFrequency, ulint_t CharDecayTime) : PulseFrequency(PulseFrequency), CharDecayTime(CharDecayTime)
  {}

  double prob(const ulint_t &simcounter) const
  {
      double elapsed_time = double(simcounter % PulseFrequency)*-1;
                      
                      //! - This is a negative exponential
      return std::exp((elapsed_time)/double(CharDecayTime));
  } //End of LaserPulseProb

}; //End of class Laser

class SimClock
{
public:

  ulint_t MaxSteps;
  double StepSize;

  //Constructor
  SimClock(ulint_t MaxSteps, double StepSize) : MaxSteps(MaxSteps), StepSize(StepSize)
  {}

}; //End of class SimClock


class Veff
{
public:
  double radius; //-----------> radius is in reduced units.
  double sf; //---------------> sf: Structure Factor for z-axis is dimensionless.
  double vol; //--------------> volume of the PSF saved.

  //static std::string type =  "3DGauss-xySymmetric"; //Polymorphic identification ?

  Veff(double radius, double sf) : radius(radius), sf(sf)
  {}

  Veff(double sf) : sf(sf), radius(0.0)
  {} // Use with the function below ↓

  double set_real_radius(double &real_radius)
  {
      this->radius = real_radius /*/ unit_conversion TODO*/;
  }

  double vol_gauss()
  {
      this->vol = CONST_PI_pow3by2 * radius * radius *  radius * sf;
      return vol;

  }

  double vol_sphere()
  {
    this->vol = 4/3*CONST_PI * radius * radius * radius;
    return vol;
  }

}; //End of class Veff

//Wrapper for all the stringstream objects → Single Interface
class Datapipe
{
    
public:

    std::ostringstream stats;
    std::ostringstream tag;

    Datapipe()
    {
      stats << std::setprecision(FCS_FLOAT_PRECISION);


      #if FCS_PART_TAGGING == 1
      tag << std::setprecision(FCS_FLOAT_PRECISION);
      #endif

    } // End of Datapipe()

    bool inline Flush(const std::string &parentpath)
    {

      bool errnox = false;
      if(!parentpath.empty())
      {
        errnox = WriteToFile(std::string(parentpath + AddExt("stats")), stats.str());


        #if FCS_PART_TAGGING == 1
        errnox && WriteToFile(std::string(parentpath + AddExt("tag")), tag.str());
        #endif
      }

      return errnox;

    } //End of Flush()

private:


  std::string AddExt(std::string str)
  {
    str.append(FCS_DATAFILE_EXT);
    return str;
  }
  
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




/*//What are the inpts? → Scale units, Viscosity of medium
class LangevinUnits
{
  
  //SI Constants → Would not change during the simulation
  double Viscosity = CONST_WATER_VISCOSITY; //Medium is the same


  double T = 273.0; //Temperatur is the same
  constexpr double kbT = T*CONST_Kb;
  constexpr double Beta = 1/kbT;

//////////////////////////////////////////////////////////////

//Declared Simulation Scale & Parameters
  double sigma = 1e-8;
  double epsilon = 10;
  double mass = 1;
  std::string time_mode = "brownian";


//Derived SI constants
  double NRho = 0.0
  double D = 0.0;
  double Gamma = 0.0;
  double TDT = 0.0; //Translational Damping Time
  double time = 0.0;


  //1/KBT

  //Brownian Time (time for a particle to diffuse the square of its diameter) → sigma*sigma/6*D
  //Translational Damping Time
  //LJ Time


  //epsilon, mass, sigma → 1
  LangevinUnits(double sigma, double epsilon, double mass) : sigma(sigma), epsilon(epsilon), mass(mass)
  {
      if(time_mode != "brownian" || time_mode != "LJ")
      {
        //Error
        exit(-1);
      }
  }


  getEnergy()
  {
    return rT * T * Kb;
  }



  getMRho()
  {
    return 
  }


  getTime()
  {
      if(time_mode == "brownian")
      {
        return sigma*sigma / (6.0 * rD * D);
      }

      else //LJ
      {
          return sigma*std::sqrt(mass*rmass/epsilon);
      }
  }


  getTranslationalDampingTime()
  {
    std::sqrt(mass*rmass*sigma*sigma/epsilon);
  }

  //getters
  //get_rMr






  double realMassFactor()
  {
    return Gamma*Gamma*sigma*sigma*Beta;
  }

  double realTimeFactor()
  {
    return Gamma*sigma*sigma*Beta;
  }


  double realForceFactor()
  {
    return kb*T/sigma;
  }

  double realDiffusivityFactor()
  {
    return Kb * T / Gamma;
  }

  double realDiffTimescaleFator()
  {
      return sigma*sigma*Beta;
  }

  //For Spherical Particles
  double realMonomerDensityFactor()
  {
      return realMassFactor()*rMass/(CONST_PI*sigma*sigma*sigma);
  }

  double realNRhoFactor()
  {
    return 1/(sigma*sigma*sigma);
  }

  double realViscosityFactor()
  {
    return Gamma/(3.0*CONST_PI*sigma);
  }


  Gamma = 3*CONST_PI*Viscosity*sigma;

};*/




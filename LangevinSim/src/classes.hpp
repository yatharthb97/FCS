//classes.hpp
#pragma once

#include <fstream>
#include<sstream>
#include<iomanip>
#include <iostream>
#include <cmath>
#include <vector>
#include <limits>
#include "macros.hpp"

#include "./submodules/filesystem.hpp"
//#include "./submodules/timer.hpp"



class Laser
{
public:

  ulint_t CharDecayTime;  //Decay time for laser
  ulint_t PulseTimePeriod; //Pulsing Time Period of laser
  
  double frequency; //Frequency of light wave
  std::string color; //Color of laser
  std::string mode = "always-on"; //Mode of operation → String val
  bool pulsing = false; //Mode of operation is pulsing → bool val


  //Constructor
  Laser(ulint_t PulseTimePeriod, ulint_t CharDecayTime) : PulseTimePeriod(PulseTimePeriod), CharDecayTime(CharDecayTime)
  {
    this->mode = "pulsing";
    pulsing = false;
  }

  //Mode Constructor
  Laser(std::string mode)
  {
    if(mode == "always-on")
    {
      this->mode = mode;
      pulsing = false;
      this->CharDecayTime = std::numeric_limits<ulint_t>::max();
      this->PulseTimePeriod = std::numeric_limits<ulint_t>::max();
    }

    else if(mode =="pulsing") //Set Default Values
    { 
        pulsing = true;
        this->mode = mode;
        this->CharDecayTime = 5;
        this->PulseTimePeriod = 10;
    }

    else
    {
      std::cerr << "[ERROR] LASER → Invalid Mode of operation: " << mode << '\n';
      std::cerr << "[•••••> LASER → Mode of operation is set : " << mode << '\n';

      this->mode = "always-on";
      this->pulsing = false;
      this->CharDecayTime = std::numeric_limits<ulint_t>::max();
      this->PulseTimePeriod = std::numeric_limits<ulint_t>::max();
    }
  } //End of Mode based Constructor

  double Prob(const ulint_t &simcounter) const
  {
      if(pulsing)
      {
        double elapsed_time = double(simcounter % PulseTimePeriod)*-1;
        return std::exp((elapsed_time)/double(CharDecayTime));
                     //! - This is a negative exponential
      }

      else //always-on
        return 1.0;

  } //End of prob


  std::string getPulseTimePeriod() const
  {
    if(!pulsing)
      return "∞";
    else
      return std::to_string(PulseTimePeriod);
  }


  std::string getCharDecayTime() const
  {
    if(!pulsing)
      return "∞";
    else
      return std::to_string(CharDecayTime);
  }

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
  double radius_sq;//---------> radius squared in reduced units.
  double sf; //---------------> sf: Structure Factor for z-axis is dimensionless.
  double vol; //--------------> volume of the PSF saved.

  //static std::string type =  "3DGauss-xySymmetric"; //Polymorphic identification ?

  Veff(double radius, double sf) : radius(radius), radius_sq(radius*radius), sf(sf)
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
    
    #if FCS_PART_TAGGING == 1
      std::ostringstream tag;
    #endif

    //distributions
    #if FCS_RND_SAMPLING == 1
      std::ostringstream u_dist;
      std::ostringstream gauss_dist;
    #endif

    //Reference of all streams
    std::vector <std::ostringstream *> pipelist;


    Datapipe()
    {
      //stats
      stats << std::setprecision(FCS_FLOAT_PRECISION);
      pipelist.emplace_back(&stats);

      //tag
      #if FCS_PART_TAGGING == 1
        tag << std::setprecision(FCS_FLOAT_PRECISION);
        pipelist.emplace_back(&tag);
      #endif

      //distributions
      #if FCS_RND_SAMPLING == 1
        pipelist.emplace_back(&u_dist);
        pipelist.emplace_back(&gauss_dist);
      #endif

    } // End of Datapipe()

    bool inline Flush(const std::string &parentpath)
    {

      bool errnox = false;
      if(!parentpath.empty())
      {
        errnox = WriteToFile(std::string(parentpath + AddExt("stats")), stats.str());


        #if FCS_PART_TAGGING == 1
        errnox = errnox && WriteToFile(std::string(parentpath + AddExt("tag")), tag.str());
        #endif


        #if FCS_RND_SAMPLING == 1
        errnox = errnox && 
                 WriteToFile(std::string(parentpath + AddExt("u_dist")), u_dist.str()) &&
                 WriteToFile(std::string(parentpath + AddExt("gauss_dist")), gauss_dist.str());
        #endif
                          
      }

      return errnox;

    } //End of Flush()

    //Clears all buffers
    void ClearAll()
    {
        for(auto stream: pipelist)
        {
          stream->str(std::string()); //Set equivalent to string of zero length
          stream->clear();  //Set error stateflags
          stream->seekp(0); // for outputs: seek put ptr to start
        }
    }

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


// UNUSED FILE
#pragma once
#include <cmath>
#include "macros.hpp"

//What are the inpts? → Scale units, Viscosity of medium
class Units
{
  public:
/////////////////////////////////////////////////////////| Member List
  
  //Declared Simulation Scale & Parameters
  double Sigma = 1e-8;
  double Epsilon = 10;
  double Mass = 1.0;

  double Time = 1.0;

  double Viscosity = CONST_WATER_VISCOSITY; //Medium is the same
  double Gamma = 3 * CONST_PI * Viscosity * Sigma; //Gamma (Inverse of Mobility)

  //Derived From Epsilon ↓
  double T = 273.0; //Temperatur is the same
  double KBT = T*CONST_Kb; //Energy Scale of the Thermal Fluctuation
  double Beta = 1/KBT;
  /////////////////////////////////////////////////////////| Member List
  

/////////////////////////////////////////////////////////| CONFIG
  
  //Blank Constructor
  Units()
  {}

  ////Inputs → Sigma, Epsilon, Mass
  void LJUnits(double Sigma, double Epsilon, double Mass)
  {
      
      this->Sigma = Sigma;
      this->Epsilon = Epsilon;
      this->Mass = Mass;

      //Set Epsilon based Attributes
      setEpsilon(Epsilon); //!!! Temperature is set twice, design choice

      //Gamma Is Unset

      /* USE ↓
         Units lj;
         lj.LJUnits(s,e,m)
         lj.getLJ_TempFactor()
         lj.getLJ_TDT()
    
      */
  }
  ////Inputs → Sigma, Epsilon, Viscosity
  void setSEV(double Sigma, double Epsilon, double Viscosity)
  {
    
      this->Sigma = Sigma;
      this->Epsilon = Epsilon;
      this->Viscosity = Viscosity;  

      //Set Epsilon based Attributes → T
      setEpsilon(Epsilon); //!!! Temperature is set twice, design choice

      //Set Gamma (Inverse of Mobility)
      this->Gamma = 3 * CONST_PI * Viscosity * Sigma;

      //Mass is implicitly set
      this->Mass = realMassFactor()*1.0;

      //Time is also implicitly set
      this->Time = realTimeFactor()*1.0;
  }


  ////Inputs → Sigma, Epsilon, Viscosity
  void setSTV(double Sigma, double Temp, double Viscosity)
  {
      
      this->Sigma = Sigma;
      this->Viscosity = Viscosity;  

      //Set Epsilon based Attributes → T
      setTemp(Temp); //!!! Temperature is set twice, design choice

      //Set Gamma (Inverse of Mobility)
      this->Gamma = 3 * CONST_PI * Viscosity * Sigma;

      //Mass is implicitly set
      this->Mass = realMassFactor()*1.0;

      //Time is also implicitly set
      this->Time = realTimeFactor()*1.0;
  }



  void setEpsilon(double epsilon)
  {
    this->KBT = epsilon;
    this->T = epsilon/CONST_Kb;
    this->Beta = 1/KBT;
  }

  void setTemp(double newT)
  {
    this->T = newT;
    this->KBT = newT*CONST_Kb;
    this->Beta = 1/KBT;
  }
  /////////////////////////////////////////////////////////| CONFIG
  

/////////////////////////////////////////////////////////| LANG UNITS
// Simulation Units Can be multiplied by the return values of the functions given below to get real SI units. ↓ "real" priffix

  //Check Again
  double realBrownianTimeFactor() //time for a particle to diffuse the square of its diameter
  {
    return Sigma * Sigma / (6.0 * realDiffusivityFactor()); //(3D form)
  }
  
  //OK
  double realVolumeFactor()
  {
    return Sigma * Sigma * Sigma;
  }

  //OK
  double realMassFactor()
  {
    return Gamma * Gamma * Sigma * Sigma * Beta;
  }

  //Ok
  double realTimeFactor()
  {
    return Gamma * Sigma * Sigma * Beta;
  }

  //OK
  double realForceFactor()
  {
    return KBT/Sigma;
  }

  //OK
  double realViscosityFactor()
  {
    return Gamma / Sigma;
  }

  //OK
  double realDiffusivityFactor()
  {
    return KBT / Gamma;
  }

  //OK
  double realDiffTSFator()  //Diffusion Time Scale Factor
  {
    return Sigma * Sigma * Beta;
  }

  //For Spherical Particles Only
  double realMonomerDensityFactor()
  {
      return realMassFactor() / realVolumeFactor();
  }
  /////////////////////////////////////////////////////////| LANG UNITS



/////////////////////////////////////////////////////////| LJ UNITS
// "get" → Preffix


    double getLJ_TDT() //For Lennard Jones System
    {
      return Sigma * std::sqrt(Mass / Epsilon);
    }


    //OK
    double getLJ_TempFactor() //For Lennard Jones
    {
      return KBT / Epsilon;
    }
  /////////////////////////////////////////////////////////| LJ UNITS

}; //End of class Units

//Notes:

  //Brownian Time (time for a particle to diffuse the square of its diameter) → sigma*sigma/6*D











//Low Reynolds number regime
namespace Environment
{
	
	static std::string medium = "Fluid";
	static bool lowReynoldsNo = true;
	static double Viscosity;
	static double Density;
	
	double ReynoldsNo(double length, double velocity)
	{
		//Re = Lvρ/η
		using Env = Environment;
		return length*velocity*Env::Density*Env::Viscosity;
	}
};


//One of the most striking aspect of low Reynolds number phenomena is that the speed of an object is solely determined by the forces acting on it at the moment.


// In particular, W(t) is almost everywhere discontinuous and has infinite variation. In an intuitive picture, it can be seen as the continuous-time equivalent of a discrete sequence of independent random numbers.
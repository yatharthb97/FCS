// UNUSED FILE
#pragma once
#include <cmath>
#include <sstream>
#include <iomanip>
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

  //Status Flags
  bool temp_fixed = false;
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
    this->temp_fixed = false;
  }

  void setTemp(double newT)
  {
    this->T = newT;
    this->KBT = newT*CONST_Kb;
    this->Beta = 1/KBT;
    this->temp_fixed = true;
  }
  /////////////////////////////////////////////////////////| CONFIG
  

/////////////////////////////////////////////////////////| LANG UNITS
// Simulation Units Can be multiplied by the return values of the functions given below to get real SI units. ↓ "real" priffix

  //Check Again
  double realBrownianTimeFactor() const//time for a particle to diffuse the square of its diameter
  {
    return Sigma * Sigma / (6.0 * realDiffusivityFactor()); //(3D form)
  }
  
  //OK
  double realVolumeFactor() const
  {
    return Sigma * Sigma * Sigma;
  } 

  //OK
  double realMassFactor() const
  {
    return Gamma * Gamma * Sigma * Sigma * Beta;
  }

  //Ok
  double realTimeFactor() const
  {
    return Gamma * Sigma * Sigma * Beta;
  }

  //OK
  double realForceFactor() const
  {
    return KBT/Sigma;
  }

  //OK
  double realViscosityFactor() const
  {
    return Gamma / Sigma;
  }

  //OK
  double realDiffusivityFactor() const
  {
    return KBT / Gamma;
  }

  //OK
  double realDiffTSFator() const//Diffusion Time Scale Factor
  {
    return Sigma * Sigma * Beta;
  }

  //For Spherical Particles Only
  double realMonomerDensityFactor() const
  {
      return realMassFactor() / realVolumeFactor();
  }
  /////////////////////////////////////////////////////////| LANG UNITS


  std::string profile(double dt, double total_steps) const
  {
    std::ostringstream buffer;
    buffer << std::setprecision(FCS_FLOAT_SHORT_PRECISION) << std::boolalpha;

    buffer << "————→   Units — Corresponding Real(SI) Unit per 1.0 Sim Unit  ←————\n\n";
    
    buffer << " • Sigma(m) : " << this->Sigma << " (Set)\n";
    buffer << " • Temp(K) : " << this->T << (temp_fixed == true ? " (Set)" : "") << '\n';
    buffer << " • Thermal Energy - KbT(J) : " << this->KBT << (temp_fixed == false ? " (Set)" : "") << '\n';
    buffer << " • Viscosity (Pa • s): " << this->Viscosity << " (Set)\n";
    buffer << " • Gamma : " << this->Gamma << "\n\n";

    buffer << " • Time(s) : " << this->Time << '\n';
    buffer << " • Time Step(s) : " << this->Time*dt << '\n';
    buffer << " • Total Simulation Time(s) : " << this->Time * total_steps << "\n\n";

    buffer << " • Mass(kg) : " << this->realMassFactor() << '\n';
    buffer << " • Force(N) : " << this->realForceFactor() << '\n';

   return buffer.str();
  }


/////////////////////////////////////////////////////////| LJ UNITS
// "get" → Preffix


    double getLJ_TDT() //For Lennard Jones System
    {
      return Sigma * std::sqrt(Mass / Epsilon);
    }


    //NOK
    double getLJ_TempFactor() //For Lennard Jones
    {
      return KBT / Epsilon;
    }
  /////////////////////////////////////////////////////////| LJ UNITS

}; //End of class Units

//Notes:

  //Brownian Time (time for a particle to diffuse the square of its diameter) → sigma*sigma/6*D






//Low Reynolds number regime
/*namespace Environment
{
	
	static std::string medium = "Fluid";
	static bool lowReynoldsNo = true;
	static double Viscosity = 0.0;
	static double Density = 0.0;
	
	double ReynoldsNo(double length, double velocity)
	{
		//Re = Lvρ/η
		using Env = Environment;
		return length * velocity * Env::Density * Env::Viscosity;
	}
};*/


//One of the most striking aspect of low Reynolds number phenomena is that the speed of an object is solely determined by the forces acting on it at the moment.


// In particular, W(t) is almost everywhere discontinuous and has infinite variation. In an intuitive picture, it can be seen as the continuous-time equivalent of a discrete sequence of independent random numbers.
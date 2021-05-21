// UNUSED FILE
#pragma once

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
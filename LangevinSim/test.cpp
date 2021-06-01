

#include "langsim.hpp"
#include <iostream>

int main()
{
	Units units;
	units.setSTV(2*1e-9, 273.16, CONST_WATER_VISCOSITY);

	std::cout << "Sigma : " << units.Sigma << '\n';

	std::cout << "Temp : " << units.T << '\n';

	std::cout << "Viscosity : " << units.Viscosity << '\n';

	std::cout << "Gamma : " << units.Gamma << '\n';

	std::cout << "Time : " << units.Time << '\n';

	std::cout << "Time * 1e-5 : " << units.Time*1e-5 << '\n';

	std::cout << "Time * 1^7 : " << units.Time * 1e7 << '\n';

	std::cout << "Mass : " << units.realMassFactor() << '\n';

	std::cout << "Force : " << units.realForceFactor() << '\n';

	std::cout << "Thermal Energy(KBT) : " << units.KBT << '\n';

	//std::cout << "Temperature" << units.T << '\n';

}
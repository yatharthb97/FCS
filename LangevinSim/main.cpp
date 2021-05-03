//Main File for Simuator

#include "classes.hpp"



int main()
{
	//LangevinBox(double Rho, unsigned int Part_no, int FrameExports)
	LangevinBox box(0.2, 50, 100, "Test");

	box.Evolve(); //Evolve Box → Run Box
}
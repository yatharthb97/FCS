//Main Test File for Langevin Simuator

#include "langsim.hpp" //Single File Interface


int main()
{
	
	
	//0. Specify HomePath(that should exist) → Optional
	HomePath("/mnt/m/code/FCS/Results/");

	//1. Create NewSession(session_name, box_mode) box_modes → "singlebox"/"multibox"
	NewSession("Test", "singlebox");

	//2. Create Simulation Clock Object → SimClock(MaxSteps, Step_Size)
	SimClock simclock(1e5, 1e-5);

	//3. Create Laser Object → (pulse interval, char, decy time)
	Laser laser(10, 5); //Pulse Every 10 dt, char decay time is 5 dt

	//4. Create Veff Object → Veff(radius, structure_factor)
	Veff veff(2.0, 2.5);

	//5. Create Box Object → LangevinBox(Rho, Part_no, FrameExports, simclock, laser, veff)
	int write_frames = 100;
	double Rho = 0.005;
	int Part_no = 200;
	
	gl::do_pos_plots = true;
	LangevinBox box(Rho, Part_no, write_frames, simclock, laser, veff);

	//6. Evolve Box → Run Box
	box.Evolve();


///////////////////// Test Features --------------------------><

	//3. Scale Units --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
	//Units units(/*sigma*/1.0, /*epsilon*/1.0, /*mass*/1.0, /*Viscosity*/CONST_WATER_VISCOSITY);
}



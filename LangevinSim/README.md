
# Langvin Simulation Code

Code for Langevin Simulation of an FCS Setup

Probablity of flashing = P(Laser Pulsing) * P(PSF) * P(Inherent Quantum Yield)


## Some Important Points:

	• **Uniform Spherical Mode** does not use PSF based probablity scaling. (Just Laser Pulse and Quantum Yield).
	• **Uniform Spherical Mode** also ignores the Structure Factor arguement (sf) passed to the Veff Object.
	• For **Uniform Spherical Mode** "PSF Exponents" and "PSF Normalization" are both set to zero.

    • FCS_RND_SAMPLING feature macro advances the state of the PRNG engine and distributions, so it will affect reporoducability in the current implementation.


## Compile via:

> g++ std=c++11 main.cpp -o langsim

## Run Via: 

> ./langsim


## Python Plot can be manually called by:

> ./Plots.py <path_to_box_directory>



## Update Scheme

	>	part.pos.x += std::sqrt(2*part.D*dt)*gauss_dist(rnd.engine);
	>   part.pos.y += std::sqrt(2*part.D*dt)*gauss_dist(rnd.engine);
	>  	part.pos.z += std::sqrt(2*part.D*dt)*gauss_dist(rnd.engine);


## Issues: 
	• "last_plot.png" is not getting rendered in python. Output is blank white image.
    • FCS_PER_PART_STATS is not implemented. Only variables are defined.
    • Particle::last_flash is initalized to zero (unsigned int). So for 0th frame, all particles flash.
    • Box Assymmetry mode is curently incompatible with python pos plot module and data fits
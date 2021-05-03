#pragma once
#include "./submodules/vector.hpp"
#include<vector>
#include<array>
/*class Particle;
using partlist_t = std::vector<Particle>;

template<std::size_t N>
using partlist_ft = std::array<Particle, N>;


using ulint_t = u_int32_t;
*/

class Particle
{
public:

	unsigned int partid;
	V pos; 		           //Position Vector
   	V init_pos;          //Initial Position
	double D = 3.0;      //Diffusion Rate
	double e_prob = 0.8; //Emmission probablity
	bool InVol = false;

	unsigned long int last_flash = 0;
	unsigned long int creation_t = 0;
	bool bleached = false;

  //Default Constructor
  Particle(unsigned int partid, V &pos): partid(partid), pos(pos), init_pos(pos)
  {}
}; //End of class Particle
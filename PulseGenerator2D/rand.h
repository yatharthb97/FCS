#pragma once
#ifdef TEENSYDUINO
  #include<Entropy.h>
#endif
namespace Rand
{
	

	unsigned int Walker_Size = 0;
	unsigned int Edge = 0;
	unsigned int StepSize = 0;

	#ifdef FCSSIM_EMBEDDEDSYSTEMS  //EMBEDDED Specific Implementation
			void Init(unsigned int walker_size, unsigned int edge, unsigned int StepSize)
			{
				 #ifdef TEENSYDUINO
				 	Entropy.Initialize();
				 	uint32_t seed_value = Entropy.random();
				 	randomSeed(seed_value);
				#else
				 	uint32_t seed_value = analogRead(FCSSIM_EMPTYENTROPYPIN);
				 	randomSeed(seed_value);
				#endif //TEENSYDUINO

				 Rand::Walker_Size = walker_size;
				 Rand::Edge = edge;
				 Rand::StepSize = StepSize+1;
			}

			int Sign()
			{
				
				unsigned int temp = random(0,2);
				temp += !(temp)*-1; //Only works if temp is 0
				return temp;

			}

			//Dynamic Step
			int DStep()
			{
				return random(0, StepSize);
			}

			//Return between 0 and Edge
			int InEdge()
			{
				return random(0, Edge);
			}
		  
		  	//Return a Random PartID/WalkerID
			unsigned int NewPart()
			{
				return random(0, Rand::Walker_Size);
			}


  #endif //Is FCSSIM_EMBEDDEDSYSTEMS

  //for Macro FCS_SIM_EMBEDDEDSYSTEMS
  #ifndef FCSSIM_EMBEDDEDSYSTEMS //Unix/Windows Specific Implementation

		RND_MT Rnd; //MT Rand Object

		//Distributions
		std::uniform_int_distribution<int> unit(0,1);
		std::uniform_int_distribution<unsigned int> partid(0,10);
		std::uniform_int_distribution<unsigned int> steps(0,5);
		std::uniform_int_distribution<unsigned int> edges(0,10);

		void Init(unsigned int walker_size, unsigned int edge, unsigned int StepSize)
		{
			
			Rand::Walker_Size = walker_size;
			Rand::Edge = edge;
			Rand::StepSize = StepSize;

			//Seeding
			Rnd.TimeSeed();

			//Param Change here

			//1
			unsigned int a = 0, int b = Rand::Walker_Size;
			partid.param(std::uniform_int_distribution<unsigned int>::param_type(a, b));

			if(!(partid.a() == a && partid.b() == b))
			{
				std::cerr << "[ERROR] Dist::partid_range() failed.\n";
				exit(0);
			}

			//2
			unsigned int c = 0, int d = Rand::StepSize;
			steps.param(std::uniform_int_distribution<unsigned int>::param_type(c, d));

			if(!(steps.a() == c && steps.b() == d))
				std::cerr << "[ERROR] Dist::steps_range() failed.\n";

			//3
			unsigned int e = 0, int f = Rand::Edge;
			edges.param(std::uniform_int_distribution<unsigned int>::param_type(e, f));

			if(!(edges.a() == e && edges.b() == f))
				std::cerr << "[ERROR] Dist::edge_range() failed.\n";
		} //End of Init()

		int Sign()
		{
				
			unsigned int temp = steps(Rnd.engine);
			temp += !(temp)*-1; //Only works if temp is 0
			return temp;

		}

		//Dynamic Step
		int DStep()
		{
			return steps(Rnd.engine);
		}

		//Return between 0 and Edge
		int InEdge()
		{
			return random(0, Edge);
		}
		  
		//Return a Random PartID/WalkerID
		unsigned int NewPart()
		{
			return partid(Rnd.engine);
		}

#endif //Not FCSSIM_EMBEDDEDSYSTEMS

}; //End of namespace Rand

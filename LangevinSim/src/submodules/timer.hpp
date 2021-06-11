//Header file for Timing Framework → <timer.hpp>
//><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><
//><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><

  /*$$$$$            /$$                                      /$$$$$$  /$$              
 /$$__  $$          | $$                                     /$$__  $$|__/              
| $$  \__/  /$$$$$$ | $$$$$$$   /$$$$$$   /$$$$$$   /$$$$$$ | $$  \__/ /$$ /$$$$$$/$$$$ 
|  $$$$$$  /$$__  $$| $$__  $$ /$$__  $$ /$$__  $$ /$$__  $$|  $$$$$$ | $$| $$_  $$_  $$
 \____  $$| $$  \ $$| $$  \ $$| $$$$$$$$| $$  \__/| $$  \ $$ \____  $$| $$| $$ \ $$ \ $$
 /$$  \ $$| $$  | $$| $$  | $$| $$_____/| $$      | $$  | $$ /$$  \ $$| $$| $$ | $$ | $$
|  $$$$$$/| $$$$$$$/| $$  | $$|  $$$$$$$| $$      |  $$$$$$/|  $$$$$$/| $$| $$ | $$ | $$
 \______/ | $$____/ |__/  |__/ \_______/|__/       \______/  \______/ |__/|__/ |__/ |__/
          | $$                                                                          
          | $$                                                                          
          |_*/
//><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><
//><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><

//><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><
//  Developed by: Yatharth Bhasin		|| Licence: *******************
//  Discipline of Physics				|| Compile : Ok -> 14/01/2021 12:30 PM
//  IIT Indore							|| Tested : NOK
//  (yatharth1997@gmail.com)			|| Documentation : NOK
//  (github: yatharthb97)				||		
//><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><



#pragma once
#include <chrono>
#include <ctime>

//TimeStamp - Create TimeStamps
std::time_t inline TimeStamp()
{
	auto stamp = std::chrono::system_clock::now();
	std::time_t stamp_time = std::chrono::system_clock::to_time_t(stamp);
	return(std::time_t (stamp_time));
}
//End of TimeStamp


class Timer
{

public:

	std::chrono::time_point<std::chrono::high_resolution_clock> stamp_time;
	//std::string key;

	//Constructor
	Timer(std::string key ="")
	{
		std::cout << "\nTiming: " << key << std::endl;
		stamp_time = std::chrono::high_resolution_clock::now();
	}


	~Timer()
	{
		using namespace std;
		std::chrono::time_point<std::chrono::high_resolution_clock> stop = std::chrono::high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(stop - stamp_time);
		std::cerr << "Timer Object Destroyed: " << key << "  \n"
			      << "Duration Elapsed:  " //<< duration.count() << " µs    "
			       //<< "= " << (double)duration.count()*0.001 << " ms    "
			       //<< "= " 
			       << (double)duration.count()*0.000001 << " s\n"
			       << std::endl;
	}

}; //End of class Timer
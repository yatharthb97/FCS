//Defines Global namespace (gl) and global functions

#pragma once

#include "macros.hpp"
#include <iostream>
#include <algorithm>
#include "modes.hpp"
#include "./submodules/filesystem.hpp"


//namespace gl (global)
namespace gl
{
	static bool session_declared = false;
	
	static std::string home_path = "./"; //TODO → OS independent implementation
	static std::string session_name;
	static std::string session_path;
	static std::string boxmode;


	static int boxcounter = -1;

	//Python Script Controls
	static bool run_python = true;
	static bool do_pos_plots = false;
	static bool show_py_plots = true;

}; //End of namespace gl (global)


	std::string inline WelcomeMessage()
	{

		const char* WelcomeBanner = R"(
		 /$$                                      /$$$$$$  /$$              
		| $$                                     /$$__  $$|__/              
		| $$        /$$$$$$  /$$$$$$$   /$$$$$$ | $$  \__/ /$$ /$$$$$$/$$$$ 
		| $$       |____  $$| $$__  $$ /$$__  $$|  $$$$$$ | $$| $$_  $$_  $$
		| $$        /$$$$$$$| $$  \ $$| $$  \ $$ \____  $$| $$| $$ \ $$ \ $$
		| $$       /$$__  $$| $$  | $$| $$  | $$ /$$  \ $$| $$| $$ | $$ | $$
		| $$$$$$$$|  $$$$$$$| $$  | $$|  $$$$$$$|  $$$$$$/| $$| $$ | $$ | $$
		|________/ \_______/|__/  |__/ \____  $$ \______/ |__/|__/ |__/ |__/
		                               /$$  \ $$                            
		                              |  $$$$$$/                            
		                               \______/                             
		)";

		//Define Additional Info

		return WelcomeBanner;

	}

	//Thread Safe → Used only once
	void static NewSession(std::string session_name, std::string boxes_hint = "singlebox")
	{
		//Set Session Name
		gl::session_name = session_name;
		
		//Print Welcome Message
		std::cout << WelcomeMessage() << std::endl;

		//Declare Modes and Conflicts
		Modes::Declare("Periodic Boundary Conditions", FCS_SYMMETRIC_BOX); //1
		Modes::Declare("Symmetric Box", FCS_ENABLE_PBC); //2
		Modes::Declare("Veff - Ellipsoid", FCS_VEFF_ELLIPSOID); //3
		Modes::Declare("InVolume Cutoff Optimization", FCS_INVOL_CUTOFF); //4
		Modes::Declare("Random Sample Test", FCS_RND_SAMPLING); //5
		
		
		if(!Modes::AllClear())
			exit(-1);

		
		//Guarentee that gl::homo_path exists
		std::string path = gl::home_path;  //TODO → Check the asymptotic conditions for MakePrimaryNode
		gl::session_path = FileSystem::MakePrimaryNode(path, session_name); //Reserve Directory
		
		//Print the actual path returned by the Node Generator
		std::cout << "The session path is set to: " << gl::session_path << std::endl;

		//Pre Process boxes_hint
		std::transform(boxes_hint.begin(), boxes_hint.end(), boxes_hint.begin(),
		    [](unsigned char c){ return std::tolower(c); });

		//Set box mode
		if(boxes_hint == "singlebox")
			gl::boxmode = boxes_hint;

		else if(boxes_hint == "multibox")
			gl::boxmode = boxes_hint;
		else
		{
			std::cerr << "Invalid box mode: " << boxes_hint << '\n';
			//TODO -> Clean the primary node or move this to top
			exit(-1);
		}

		gl::session_declared = true;

	} //End of function NewSession()

	//Sets the gl::homepath variable
	//Thread Safe → Used only once
	void HomePath(std::string hpath)
	{
		FileSystem::SlashIt(hpath);

		if(FileSystem::dirExists(hpath) <= 0)
		{
			std::cerr << "> [FATAL ERROR] Home Path directory does not exist.\n\t\t\tPlease create the directory: " << hpath << '\n';
			exit(-1);
		}

		else
		{
			gl::home_path = hpath;
		}
	} // End of function HomePath()
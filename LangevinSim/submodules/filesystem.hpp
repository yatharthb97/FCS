//Header file for namespace FileSystem → <filesystem.hpp>
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

#include <fstream>
#include <sstream>
#include <sys/stat.h>

namespace FileSystem
{
	//static std::string parentpath;

	bool GenerateFile(const std::string &parentpath, const std::string &filename, const std::string &content)
	{
		std::string path(parentpath);
		path.append("/");
		path.append(filename);

		std::fstream filex(path, std::ios::out);

		if(filex.is_open())
		{
			filex << content;
			return true;
			//RAII file close expected. 
		}

		else
			return false;
	}


	bool fileExists(const std::string name) 
	{
	  struct stat buffer;   
	  return (stat (name.c_str(), &buffer) == 0); 
	} //End of FILEexists()



	/******************************************************************************
	 * Checks to see if a directory exists. Note: This method only checks the
	 * existence of the full path AND if path leaf is a dir.
	 *
	 * @return  >0 if dir exists AND is a dir,
	 *           0 if dir does not exist OR exists but not a dir,
	 *          <0 if an error occurred (errno is also set)

	 Source :: https://stackoverflow.com/a/52043954/11733629
	 *****************************************************************************/
	int dirExists(const std::string &path)
	{
	    struct stat info;

	    int statRC = stat( path.c_str(), &info );
	    if( statRC != 0 )
	    {
	        if (errno == ENOENT)  { return 0; } // something along the path does not exist
	        if (errno == ENOTDIR) { return 0; } // something in path prefix is not a dir
	        return -1;
	    }

	    return ( info.st_mode & S_IFDIR ) ? 1 : 0;
	} // End of dirExists()




	bool inline Is_slash(const char t)
	{
		return (t == '/');
	}

	void inline SlashIt(std::string &line)
	{
		if(!(line.back() == '/'))
			line.append("/");
	}




	//Returns a slashed path
	std::string MakePrimaryNode(std::string &parentpath, std::string &node_name)
	{
		
		
		std::ostringstream mkd;
		//If not slash include slah

		//Construct Path
		mkd << parentpath;

		if(!FileSystem::Is_slash(mkd.str().back()))
			mkd << '/';

		mkd << node_name;
		

		//Find a unique path recursively
		int checkdir = FileSystem::dirExists(mkd.str().c_str());
		std::string aprovedpath = mkd.str() + "/";
		int dupcounter = 1;
		
		while(checkdir > 0)
		{
			std::ostringstream x;
			x << mkd.str() << "_" << dupcounter << '/';
			aprovedpath = x.str();
			checkdir = FileSystem::dirExists(aprovedpath.c_str());
			dupcounter++;
		}

		mkdir(aprovedpath.c_str(), 0777);
		return aprovedpath;
	}


};
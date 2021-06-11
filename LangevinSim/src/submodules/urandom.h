//C code for urandom ==> seed by system entropy → <urandom.h>
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


 ///Include licence information

/*
//Max numbers that can be read
//Block Error
//Entropy Production Rate of Machine

*/

//Preprocessors
#pragma once
#include<unistd.h>
#include<fcntl.h>
#include<iostream>


//class Urandom
//<template typename T>
class Urandom
{

	//int read_size;
	int fd; //File descriptor for open
	bool open_flag;

public:

	//Constructor -> Sets the read size
	Urandom(): fd(-1), open_flag(false)
	{
		//read_size = sizeof(T);

	}


	~Urandom()
	{
		if(open_flag)
		{
			close(fd);
		}
	}

	bool inline Open()
	{
		fd = open("/dev/urandom", O_RDONLY, O_NONBLOCK); //Non-blocking mode implemented

		if (fd != -1)
		{
			open_flag = true;
			return true;
		}

		else
			return false;
	}

	bool inline is_open()
	{
		return open_flag;
	}



	void inline Close()
	{
		open_flag = false;
		fd = -1;
		close(fd);
	}

	template <typename T>
	T inline get()
	{
		if(open_flag)
		{
			T read_value;
			ssize_t r = read(fd, &read_value, sizeof(T));
			return read_value;
		}

		else
		{
			std::cerr << "Urandom: urandom file not open!\n";
			exit(EXIT_FAILURE);
		}
	}

};//End of class Urandom


//Must define an open() method and a get() method





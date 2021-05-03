//Simulator
#pragma once

// Libraries  ------->



//Ask for session name
//Create directory
//Construct Parent Path
//Run Script

//Frame 0 is constructed in setup
//Pyhton script is launched via a system call



// Macro definations ------------------> 
#define FCSSIM_DEFAULT_EDGE 100
#define FCSSIM_MULTI_PART_STATE 0
#define FCSSIM_ERRORS 0
#define FCS_SIM_DSEP ','
#define FCSSIM_FRAME_EXPORT 0



//Flags
bool DirectorySet = false;



//Global --------------------------------------------------------------


std::string temp_file = "./tempfile.dat";







//global
#if FCSSIM_FRAME_EXPORT == 1
bool DirectorySet = false;
std::string parent_path;
#endif //FCSSIM_FRAME_EXPORT


if(measureIndex%FCSSIM_FRAMEINTERVAL == 0)
	VisualExport();


#if FCSSIM_FRAME_EXPORT == 1 && defined(FCSSIM_EMBEDDEDSYSTEMS)
	
	void inline SlashIt(std::string &line)
	{
		if(!(line.back() == '/'))
			line.append("/");
	} //End of SlashIt


	void VisualExport()
	{
		
		ExportIndex++; //Increment ExportIndex		

		if(!DirectorySet)
		{

			std::ostringstream buffer;

			for(unsigned int i = 0; i < Walker_No; i++)
			{
				buffer << i << FCSSIM_DSEP << POSx[i] << FCSSIM_DSEP << POSy[i] << '\n'; 
			}

			std::string grand_parentpath = "/mnt/m/code/FCSResults/"; //Fixed at compiletime
			std::string session_name;
			if (argc == 0)
			{
				std::cerr << "Error: No Session name given! Enter it after the script name." << std::endl;
				exit(EXIT_FAILURE);
			}

			else
			{
				session_name = argv[1];
			}

			parent_path = SlashIt(grand_parentpath).append(session_name);
			SlashIt(parent_path);
			//No Safety
			if(mkdir(parent_path, 0777) != -1)
				DirectorySet = true;
			
		}


		std::string filename = parent_path.append(std::tostring(ExportIndex));
		filename.append(".dat");
		std::fstream file(filename, std::ios::out);
		file << buffer.str();
		file.close();
	

	//Assume Python plot instance is already opened by Init()
}
#endif // FCSSIM_FRAME_EXPORT && FCSSIM_EMBEDDEDSYSTEMS







#pragma once
#include <fstream>
#include<sstream>
#include<iomanip>
#include <iostream>
#include <utility>
#include <vector>

namespace Modes{

  static std::ostringstream temp_buffer;
  static std::ostringstream main_buffer;
  static unsigned int mode_counter;

//public:

  //static std::tuple<std::string, bool> mode;
  static std::vector<std::pair<std::string, bool>> modelist;
  static bool buffer_cout;
  static bool conflicts;
  static std::string modecode;


  void static Push_back(std::string mode_str, bool mode)
  {
    modelist.push_back(std::make_pair(mode_str, mode));
    mode_counter++;

      temp_buffer.str(std::string());
      temp_buffer.clear();
      temp_buffer.seekp(0); // for outputs: seek put ptr to start
      temp_buffer << std::setw(2) << std::setfill(' ') << std::boolalpha;

      temp_buffer << mode_counter << "." << mode_str << " -> " << mode << '\n';

      main_buffer << temp_buffer.str();

      if(buffer_cout)
        std::cout << temp_buffer.str();

  }



  void static RaiseConflict(bool mode1, bool mode2, std::string conflictmsg, std::string resolutionmsg="")
  {
    if(mode1 != mode2)
    {
      conflicts = true;
      //Clear Buffer
      temp_buffer.str(std::string());
      temp_buffer.clear();
      temp_buffer.seekp(0); // for outputs: seek put ptr to start
      //temp_buffer << std::setprecision(SpS_PRECISION);  

        temp_buffer << "[CONFLICT] "<< conflictmsg << '\n';
        if(!resolutionmsg.empty())
        temp_buffer  << "\t[RESOLVE]-> " << resolutionmsg << '\n';

        main_buffer << temp_buffer.str();

        if(buffer_cout)
          std::cout << temp_buffer.str();
    }     
  }


  void Check(bool condition, std::string conflictmsg, std::string resolutionmsg)
  {
    if(!condition)
    {
      conflicts = true;
      //Clear Buffer
      temp_buffer.str(std::string());
      temp_buffer.clear();
      temp_buffer.seekp(0); // for outputs: seek put ptr to start
      //temp_buffer << std::setprecision(SpS_PRECISION);  

        temp_buffer << "[CONFLICT] "<< conflictmsg << '\n';
        if(!resolutionmsg.empty())
          temp_buffer  << "\t[RESOLVE]-> " << resolutionmsg << '\n';
        
        main_buffer << temp_buffer.str();

        if(buffer_cout)
          std::cout << temp_buffer.str();
    }     
  }


  void ModeFile(const std::string &parent_path)
  {
    
    main_buffer << "Mode Code -> " << ModeCode() << '\n';

    if(buffer_cout)
      std::cout << "Mode Code -> " << ModeCode() << std::endl;

    std::string path = parent_path;
    path.append("modelog.txt");

    std::ofstream file(path, std::ios::out);
    file << main_buffer.str();
    //RAII file close expected
  }

  
  std::string ModeCode()
  {
      if(modecode.empty())
      {
        //Clear Temp Buffer
        temp_buffer.str(std::string());
        temp_buffer.clear();
        temp_buffer.seekp(0); // for outputs: seek put ptr to start
        temp_buffer << std::noboolalpha;

        for(auto &modex: modelist)
        {
          temp_buffer << modex.second;
        }

        return temp_buffer.str();
      }

      else return modecode;

  }


}; //End of Modes
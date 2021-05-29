#pragma once
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <utility>
#include <vector>
#include <tuple>
#include <string>
#include <cmath>

namespace Modes{

  static std::ostringstream temp_buffer;
  static std::ostringstream main_buffer;
  static unsigned int mode_counter = 0;


  //static std::tuple<std::string, bool> mode;
  static std::vector<std::pair<std::string, bool>> modelist;
  static bool buffer_cout = true;
  static bool conflicts = false;

  static unsigned int modecode = 0;
  static std::string binary_code;


  void ClearTempBuffer()
  {
    //Clear Temp Buffer
    Modes::temp_buffer.str(std::string());
    Modes::temp_buffer.clear();
    Modes::temp_buffer.seekp(0); // for outputs: seek put ptr to start
    
  }


  void static Declare(std::string mode_str, bool mode)
  {
    //Add a mode
    modelist.push_back(std::make_pair(mode_str, mode));
    mode_counter++;


    Modes::ClearTempBuffer();
    Modes::temp_buffer << std::boolalpha;
    temp_buffer << "> MODE " << mode_counter << ". " << mode_str << " -> " << mode << '\n';
    
    main_buffer << temp_buffer.str();
    if(buffer_cout) //Set Output on screen
      std::cout << temp_buffer.str() << std::flush;

  }


  void static RaiseConflict(bool mode1, bool mode2, std::string conflictmsg, std::string resolutionmsg="")
  {
    if(mode1 != mode2)
    {
      conflicts = true;
      
      Modes::ClearTempBuffer(); 

      temp_buffer << "[CONFLICT] "<< conflictmsg << '\n';
      
      if(!resolutionmsg.empty())
        temp_buffer  << "\t[RESOLVE]-> " << resolutionmsg << '\n';

        main_buffer << temp_buffer.str();

      if(buffer_cout)
        std::cout << temp_buffer.str() << std::flush;
    }     
  }


  void Check(bool condition, std::string conflictmsg, std::string resolutionmsg)
  {
    if(!condition)
    {
      conflicts = true;
      
      Modes::ClearTempBuffer();
     
      temp_buffer << "[CONFLICT] "<< conflictmsg << '\n';
      
      if(!resolutionmsg.empty())
        temp_buffer  << "\t[RESOLVE]-> " << resolutionmsg << '\n';
        
        main_buffer << temp_buffer.str();

        if(buffer_cout)
          std::cout << temp_buffer.str() << std::flush;
    }     
  }


  void GenModeCode()
  {  
      Modes::ClearTempBuffer();
      unsigned int calcmodecode = 0;
      for(unsigned i = 0; i < modelist.size(); i++)
      {
        temp_buffer << std::to_string(Modes::modelist[i].second);
        calcmodecode += Modes::modelist[i].second * std::pow(2,i);
      }

      Modes::binary_code =  temp_buffer.str();
      Modes::modecode = calcmodecode;
  }

  void ModeFile(const std::string &parent_path)
  {
    Modes::GenModeCode();
    Modes::ClearTempBuffer();
    temp_buffer << "Mode Code -> " << Modes::modecode << '\n';
    temp_buffer << "Binary Mode Code -> " << Modes::binary_code << '\n';
    main_buffer << temp_buffer.str();

    if(buffer_cout)
      std::cout << temp_buffer.str() << std::flush;

    std::string path = parent_path;
    path.append("/modelog.txt");

    std::ofstream file(path, std::ios::out);
    file << main_buffer.str();
    //RAII file close expected
  }


  std::tuple<unsigned int, std::string> ModeCode()
  {
    Modes::GenModeCode();
    return std::make_tuple(Modes::modecode, Modes::binary_code);
  } 


  void SetOperation(std::string op)
  {
    if(op == "quiet")
    {
      Modes::buffer_cout = false;
    }

    else if(op == "verbose")
    {
      Modes::buffer_cout = true;
    }
  }

  std::string ModeCodePrint()
  {
    Modes::GenModeCode();
    Modes::ClearTempBuffer();
    temp_buffer << "Mode Code -> " << Modes::modecode << '\n';
    temp_buffer << "Binary Mode Code -> " << Modes::binary_code << '\n';
    return temp_buffer.str();
  }





}; //End of Modes
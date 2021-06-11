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

/* M O D E S

• Macro Mode Manager → Modes

Describes a namespace modes. It is declared as a namespace and not a class because Macros are global and cannot be scoped.

High level Macro Info Manager. Provides an interface to Declare, Check, and Raise Constraint Conflicts for better Program/Feature Management. 

---
Author : Yatharth Bhasin
Licence : MIT Licence (Text → https://opensource.org/licenses/MIT)
---

*/

namespace Modes {

  static std::ostringstream temp_buffer;
  static std::ostringstream main_buffer;

  //static std::tuple<std::string, bool> mode;
  static std::vector<std::pair<std::string, bool>> modelist;
  static bool buffer_cout = true;
  static bool conflicts = false;
  
  static unsigned int mode_counter = 0;
  static unsigned int modecode = 256;
  static unsigned int req_modecode = 0;
  
  static std::string binary_code;
  static bool modecode_conflict = false;

  void ClearTempBuffer()
  {
    //Clear Temp Buffer
    Modes::temp_buffer.str(std::string());
    Modes::temp_buffer.clear();
    Modes::temp_buffer.seekp(0); // for outputs: seek put ptr to start 
  } // End of Modes::ClearTempBuffer()


  void static Declare(std::string mode_str, bool mode)
  {
    //Add a mode
    modelist.push_back(std::make_pair(mode_str, mode));
    mode_counter++;

    Modes::ClearTempBuffer();
    
    Modes::temp_buffer << std::boolalpha;
    temp_buffer << "> [MODE] :" << mode_counter << ". " << mode_str << " → " << mode << '\n';
    
    main_buffer << temp_buffer.str();

    if(buffer_cout) //Set Output on screen
      std::cout << temp_buffer.str() << std::flush;
  }// End of Modes::Declare()


  void static RaiseConflict(bool mode1, bool mode2, std::string conflictmsg, std::string resolutionmsg="")
  {
    if(mode1 != mode2)
    {
      conflicts = true;
      Modes::ClearTempBuffer(); 

      temp_buffer << "> [CONFLICT] "<< conflictmsg << '\n';
      
      if(!resolutionmsg.empty())
        temp_buffer  << "\t\t[RESOLVE] → " << resolutionmsg << '\n';
        
      main_buffer << temp_buffer.str();

      if(buffer_cout)
        std::cout << temp_buffer.str() << std::flush;
    }     
  }// End of Modes::RaiseConflict()


  void static Check(bool condition, std::string conflictmsg, std::string resolutionmsg="")
  {
    if(!condition)
    {
      conflicts = true;
      Modes::ClearTempBuffer();
     
      temp_buffer << "> [CONFLICT] "<< conflictmsg << '\n';
      
      if(!resolutionmsg.empty())
        temp_buffer  << "\t\t[RESOLVE] → " << resolutionmsg << '\n';
        
      main_buffer << temp_buffer.str();

      if(buffer_cout)
        std::cout << temp_buffer.str() << std::flush;
    }     
  }// End of Modes::Check()


  void static GenerateCode()
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
  }// End of Modes::GenerateCode()

  void static SaveLogs(const std::string &parent_path)
  {
    Modes::GenerateCode();
    Modes::ClearTempBuffer();

    temp_buffer << "Mode Code → " << Modes::modecode << '\n';
    temp_buffer << "Binary Representation → " << Modes::binary_code << '\n';
    main_buffer << temp_buffer.str();

    if(buffer_cout)
      std::cout << temp_buffer.str() << std::flush;

    std::string path = parent_path;
    path.append("/modelog.txt");

    std::ofstream file(path, std::ios::out);
    file << main_buffer.str();
    //RAII file close expected
  }// End of SaveLogs()


  std::tuple<unsigned int, std::string> static GetCode()
  {
    Modes::GenerateCode();
    return std::make_tuple(Modes::modecode, Modes::binary_code);
  }// End of GetCode()


  void static SetAttribute(std::string op)
  {
    if(op == "quiet")
    {
      Modes::buffer_cout = false;
    }

    else if(op == "verbose")
    {
      Modes::buffer_cout = true;
    }

    else if(op == "clearmodes" || op == "reset")
    {
      Modes::ClearTempBuffer();

      //Clear Main Buffer
      Modes::main_buffer.str(std::string());
      Modes::main_buffer.clear();
      Modes::main_buffer.seekp(0);

      modecode = 256;
      binary_code = "";
      conflicts = false;
      modecode_conflict = false;

    }
  }// End of Modes::CheckModeCode()

  std::string static GetCodeStr()
  {
    Modes::GenerateCode();
    Modes::ClearTempBuffer();
    temp_buffer << "> [MODES] Mode Code → " << Modes::modecode << '\n';
    temp_buffer << "> [MODES] Binary Mode Code → " << Modes::binary_code << '\n';
    return temp_buffer.str();
  }// End of Modes::GetCodeStr()


  void static SetModeCode(int code)
  {
    Modes::req_modecode = code;
    std::cerr << "Macro Mode requirement set to : " << req_modecode << "\n";
  }// End of Modes::SetModeCode()


  void static CheckModeCode()
  {
    if(req_modecode != modecode)
    {
      Modes::modecode_conflict = true;
      
      if(buffer_cout)
      {
        std::cerr << "[MODES ERROR] Mode Code requirement not fulfilled for code :" << req_modecode << '\n'
                  << "[===========> Check Macro Feature Modes. Current Mode: " << modecode << "\n"; 
      }
      
    }

    else
    {
      if(buffer_cout)
      {
        std::cerr << "> [MODES] Macro Mode requirement fulfilled: Code "<< modecode << "\n";
      }
    } 
  }// End of Modes::CheckModeCode()

  bool static AllClear()
  {
    Modes::GenerateCode();
    Modes::CheckModeCode();
    return !conflicts && !modecode_conflict;
  }// End of Modes::AllClear()

}; //End of Modes
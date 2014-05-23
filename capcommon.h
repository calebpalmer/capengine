#ifndef CAPCOMMON_H
#define CAPCOMMON_H

#include <string>

namespace CapEngine {
#ifdef WIN
#define LINESEP "\r\n"
#endif
#ifdef UNIX
#define LINESEP "\n"
#endif
#ifdef OSX
#define LINESEP "\r"
#endif
  
  void sleep(int ms);
  std::string strip(const std::string str);
  std::string replace(const std::string str, const std::string rep, const std::string val); 
}

#endif
 

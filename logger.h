#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <ostream>
#include <iostream>

namespace CapEngine {
  
  class Logger {
    static bool instantiated;
    std::ostream* outputStream;

    
  public:
    enum LogLevel {
      CNONE = 0,
      CDEBUG,
      CWARNING,
      CERROR,
    };
    
    LogLevel currentLevel;

    Logger();
    ~Logger();

    void log(const std::string& message, LogLevel level, char const* in_file="", int in_lineno=-1);
    void setOutput(std::ostream& stream);
    
  private:
    std::string getLevelDescription(LogLevel level);
    
 };


  
}


#endif // LOGGER_H

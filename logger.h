#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <ostream>
#include <iostream>

namespace CapEngine {
  
  class Logger {
    static Logger* instance;
    std::ostream* outputStream;

    Logger();
    Logger(const Logger& logger);
    Logger& operator=(const Logger& logger);
    
  public:
    enum LogLevel {
      CDEBUG,
      CWARNING,
      CERROR
    };
    
    LogLevel currentLevel;

    ~Logger();
    static Logger& getInstance();
    void log(const std::string& message, LogLevel level);
    void setOutput(std::ostream& stream);
    
  private:
    std::string getLevelDescription(LogLevel level);
    
 };
  
}


#endif // LOGGER_H

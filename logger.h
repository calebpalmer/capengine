#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <ostream>
#include <iostream>

#define CAP_LOG(logger, msg, level)	\
	logger->log(msg, level, __FILE__, __LINE__)

#define CAP_LOG_SDL_ERROR(logger, level) \
	logger->logSDLError(level, __FILE__, __LINE__);

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

    void log(const std::string& message, LogLevel level = CWARNING, char const* file="", int lineno=-1);
		void log(const std::exception &e, LogLevel level = CWARNING, char const* file="", int lineno=-1);
		void logSDLError(LogLevel level, char const* file="", int lineno=-1);
    void setOutput(std::ostream& stream);
    
  private:
    std::string getLevelDescription(LogLevel level);
    
 };


  
}


#endif // LOGGER_H

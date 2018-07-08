#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <ostream>
#include <iostream>
#include <jsoncons/json.hpp>

#define CAP_LOG(logger, msg, level) \
	assert(logger != nullptr); \
	logger->log(msg, level, __FILE__, __LINE__);

#define CAP_LOG_SDL_ERROR(logger, level) \
	assert(logger != nullptr); \
	logger->logSDLError(level, __FILE__, __LINE__);

#define CAP_LOG_EXCEPTION(logger, e, level) \
	assert(logger != nullptr); \
	logger->log(e, level, __FILE__, __LINE__);

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
		void log(const jsoncons::json_exception &e, LogLevel = CWARNING, char const* file="", int lineno=-1);
		void logSDLError(LogLevel level, char const* file="", int lineno=-1);
    void setOutput(std::ostream& stream);
    
  private:
    std::string getLevelDescription(LogLevel level);
    
 };


  
}


#endif // LOGGER_H

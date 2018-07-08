#include "logger.h"
#include "CapEngineException.h"

#include <memory>
#include <sstream>
#include <SDL2/SDL.h>

using namespace CapEngine;
using namespace std;

bool Logger::instantiated = false;

Logger::Logger(){
  outputStream = &cout;
  currentLevel = Logger::CNONE;
}

Logger::~Logger(){
}

string Logger::getLevelDescription(Logger::LogLevel level){
  switch(level){
  case CNONE:
    return "NONE";
    break;
  case CDEBUG:
    return "DEBUG";
    break;
  case CWARNING:
    return "WARNING";
    break;
  case CERROR:
    return "ERROR";
    break;
  default:
    throw CapEngineException("Unknown logging level");
  }
}

void Logger::log(const string& message, LogLevel level,
                char const* in_file, int in_lineno){
  if(level >= currentLevel){
    ostringstream errorStream;
    string errorLevelDesc = getLevelDescription(level);
    errorStream << errorLevelDesc << ": " << message;
    
    if(std::string(in_file) != "" && in_lineno != -1)
      errorStream << " (" << in_file << ":" << in_lineno << ")";  

    *outputStream << errorStream.str() << endl;
  }
}



//! Log an exception
/** 
 \param e - The exception to log.
 \param level - The log level
 \param file - The file this is logged from
 \param lineno - The line number where this is logged from.
 \return 
*/
void Logger::log(const std::exception &e, LogLevel level, char const* file, int lineno){
	std::stringstream msg;
	msg << boost::diagnostic_information(e);

	this->log(msg.str(), level, file, lineno);
}

//! Log a json exception
/** 
 \param e - The exception to log.
 \param level - The log level
 \param file - The file this is logged from
 \param lineno - The line number where this is logged from.
 \return 
*/
void Logger::log(const jsoncons::json_exception &e, LogLevel level, char const* file, int lineno){
	this->log(e.what(), level, file, lineno);
}


//! log an SDL error code.
/** 
 \param level - The level to log it under.
 \param file - The file where the error occured.
 \param luneno - The lineno where it occured.
 \return 
*/
void Logger::logSDLError(LogLevel level, char const* file, int lineno){
	const char* error = SDL_GetError();
	std::ostringstream msg;
	msg << "SDL Error ocurred: " << error;
	this->log(msg.str(), level, file, lineno);
}

void Logger::setOutput(ostream& stream){
  outputStream = &stream;
}

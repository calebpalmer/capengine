#include "logger.h"

#include <memory>
#include <sstream>

#include "CapEngineException.h"

using namespace CapEngine;
using namespace std;

Logger* Logger::instance = nullptr;

Logger::Logger(){
  outputStream = &cout;
  currentLevel = Logger::CDEBUG;
}

Logger::~Logger(){
}

string Logger::getLevelDescription(Logger::LogLevel level){
  switch(level){
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

Logger& Logger::getInstance(){
  if(instance == nullptr){
    unique_ptr<Logger> sp_logger(new Logger());
    instance = sp_logger.release();
  }
  return *instance;
}

void Logger::log(const string& message, LogLevel level){
  if(level >= currentLevel){
    ostringstream errorStream;
    string errorLevelDesc = getLevelDescription(level);
    errorStream << errorLevelDesc << ": " << message;
    *outputStream << errorStream.str() << endl;
  }
}

void Logger::setOutput(ostream& stream){
  outputStream = &stream;
}

#include "logger.h"

#include <memory>
#include <sstream>

#include "CapEngineException.h"

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

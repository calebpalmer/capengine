#ifndef CAPENGINEEXCEPTION_H
#define CAPENGINEEXCEPTION_H

//@brief - Define exceptions for CapEngine
#include <stdexcept>
#include <string>

namespace CapEngine{
  //class for time exceptions.  It is a runtime error
  class CapEngineException : public std::runtime_error {
  public:
    explicit CapEngineException(const std::string& msg) : std::runtime_error(msg){}
  };

}

#define CAP_THROW_NULL(expr, msg) if(expr == nullptr) \
  throw new CapEngine::CapEngineException(msg);

#endif

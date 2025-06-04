#ifndef CAPENGINEEXCEPTION_H
#define CAPENGINEEXCEPTION_H

//@brief - Define exceptions for CapEngine
#include <boost/exception/all.hpp>
#include <stacktrace>
#include <stdexcept>
#include <string>

namespace CapEngine {
// class for time exceptions.  It is a runtime error
class CapEngineException : public std::runtime_error {
   public:
    explicit CapEngineException(const std::string& msg) : std::runtime_error(msg), m_trace(std::stacktrace::current())
    {
    }

    [[nodiscard]] const std::stacktrace& trace() const noexcept { return m_trace; }

   private:
    std::stacktrace m_trace;
};

}  // namespace CapEngine

#define CAP_THROW(e) BOOST_THROW_EXCEPTION(e);

#define CAP_THROW_NULL(expr, msg) \
    if ((expr) == nullptr) BOOST_THROW_EXCEPTION(CapEngine::CapEngineException(msg));

#define CAP_THROW_ASSERT(expr, msg) \
    if ((expr) == false) BOOST_THROW_EXCEPTION(CapEngine::CapEngineException(msg));

#endif

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

    [[nodiscard]] const std::stacktrace& trace() const noexcept
    {
        return m_trace;
    }

   private:
    std::stacktrace m_trace;
};

}  // namespace CapEngine

// MSVC-safe variadic macro dispatch
#define GET_VA_MACRO(_1, _2, NAME, ...) NAME

#define CAP_THROW(e) BOOST_THROW_EXCEPTION(e);

#define CAP_THROW_NULL_2(expr, msg) \
    if ((expr) == nullptr)          \
        BOOST_THROW_EXCEPTION(CapEngine::CapEngineException(msg));

#define CAP_THROW_NULL_1(expr) \
    if ((expr) == nullptr)     \
        BOOST_THROW_EXCEPTION(CapEngine::CapEngineException("Null pointer"));

#define CAP_THROW_NULL(...) GET_VA_MACRO(__VA_ARGS__, CAP_THROW_NULL_2, CAP_THROW_NULL_1)(__VA_ARGS__)

#define CAP_THROW_ASSERT_2(expr, msg) \
    if ((expr) == false)              \
        BOOST_THROW_EXCEPTION(CapEngine::CapEngineException(msg));

#define CAP_THROW_ASSERT_1(expr) \
    if ((expr) == false)         \
        BOOST_THROW_EXCEPTION(CapEngine::CapEngineException("CapEngine Assertion Failed"));

#define CAP_THROW_ASSERT(...)                                         \
    GET_VA_MACRO(__VA_ARGS__, CAP_THROW_ASSERT_2, CAP_THROW_ASSERT_1) \
    (__VA_ARGS__)

#define CAP_THROW_SDL_ERROR() BOOST_THROW_EXCEPTION(CapEngineException(SDL_GetError()));

#endif

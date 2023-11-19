#ifndef CAPENGINE_DEFER_H
#define CAPENGINE_DEFER_H

#include <functional>

namespace CapEngine
{

class Defer
{
   public:
    Defer(std::function<void()> f) : m_exitFunc(f) {}

    ~Defer()
    {
        try
        {
            m_exitFunc();
        }
        catch (...)
        {
            nullptr;  // eat it for now
        }
    }

   private:
    std::function<void()> m_exitFunc;
};
}  // namespace CapEngine

#endif  // CAPENGINE_DEFER_H

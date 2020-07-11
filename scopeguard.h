#ifndef CAPENGINE_SCOPEGUARD_H
#define CAPENGINE_SCOPEGUARD_H

#include <functional>

namespace CapEngine
{

class ScopeGuard
{
public:
  ScopeGuard(std::function<void()> f) : m_exitFunc(f) {}

  ~ScopeGuard()
  {
    try {
      m_exitFunc();
    } catch (...) {
      nullptr; // eat it for now
    }
  }

private:
  std::function<void()> m_exitFunc;
};
} // namespace CapEngine

#endif // CAPENGINE_SCOPEGUARD_H

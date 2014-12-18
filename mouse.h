#ifndef MOUSE_H
#define MOUSE_H

#include <vector>
#include "vector.h"

namespace CapEngine {
  class Mouse{
  public:
    Mouse(int numButtons);
    int getx() const;
    int gety() const;
    void setx(int x);
    void sety(int y);
    bool getButtonState(unsigned int buttonIndex) const;
    void setButtonState(unsigned int buttonIndex, bool state);
  private:
    std::vector<bool> m_buttonStates;
    CapEngine::Vector m_position;
  };
}
#endif // MOUSE_H

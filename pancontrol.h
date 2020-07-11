#ifndef CAPENGINE_PANCONTROL_H
#define CAPENGINE_PANCONTROL_H

#include "control.h"

namespace CapEngine
{
namespace UI
{

class PanControl : public Control
{
public:
  PanControl(int x, int y);

  void handleMouseButtonEvent(SDL_MouseButtonEvent mouseButtonEvent) override;

  void setHandled(bool handled) override;
  bool isHandled() const override;

  void setLocation(int x, int y);
  std::pair<int, int> getLocation();

private:
  int m_x = 0;         //<! The x location of the start of the pan.
  int m_y = 0;         //<! The y location of the start of the pan.
  bool m_done = false; //<! flag indicating if this control is done
};

} // namespace UI
} // namespace CapEngine

#endif // CAPENGINE_PANCONTROL_H

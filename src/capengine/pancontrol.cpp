#include "pancontrol.h"

namespace CapEngine
{
namespace UI
{

//! Constructor
/**
 \param x
   The x location of when panning started.
 \param y
   The y location of when panning startedd.
*/
PanControl::PanControl(int x, int y) : m_x(x), m_y(y) {}

//! \copydoc Control::setHandled
void PanControl::setHandled(bool handled) { m_done = handled; }

//! \copydoc Control::isHandled
bool PanControl::isHandled() const { return m_done; }

//! Gets the last location of the pan.
/**
 \return
   The last location of the pan.
*/
std::pair<int, int> PanControl::getLocation()
{
  return std::make_pair(m_x, m_y);
}

//! Sets a new location for the pan control
/**
 \param x
   The new x location for the pan control
 \param y
   The new y location for the pan control.
*/
void PanControl::setLocation(int x, int y)
{
  m_x = x;
  m_y = y;
}

//! \copydoc Widget::handleMouseButtonEvent
void PanControl::handleMouseButtonEvent(SDL_MouseButtonEvent event)
{
  if (event.type == SDL_MOUSEBUTTONUP && event.button == SDL_BUTTON_LEFT) {
    this->setHandled(true);
  }
}

} // namespace UI
} // namespace CapEngine

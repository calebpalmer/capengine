#ifndef CAPENGINE_CONTROL_H
#define CAPENGINE_CONTROL_H

#include "widget.h"

namespace CapEngine
{
namespace UI
{

// This class is a UI control.  For now controls what the mouse is doing.
class Control : public Widget
{
public:
  virtual ~Control() = default;

  virtual SDL_Rect getPosition() const override { return {}; }

  //! \copydoc Widget::setPosition
  virtual void setPosition(int, int) override{};

  //! \copydoc Widget::setSize
  virtual void setSize(int, int) override{};

  //! \copydoc Widget::render
  virtual void render() override {}

  //! Sets if the control has been handled or not.
  /**
                  \param handled
                    True if it is handled, false otherwise.
  */
  virtual void setHandled(bool /*handled*/){};

  //! check to see if control has been handled or not.
  /**

                  This is intended to be used by the owner of the control
                  to determine whether the control should be destroyed.

                  \return
                  true if the control has been handled, false otherwise.
  */
  virtual bool isHandled() const { return false; }
};

} // namespace UI
} // namespace CapEngine

#endif // CAPENGINE_CONTROL_H

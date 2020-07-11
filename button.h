#ifndef BUTTON_H
#define BUTTON_H

#include "uiobject.h"
#include <string>

namespace CapEngine
{

class Button : public UIObject
{
public:
  virtual ~Button() {}
  virtual std::string getText() const = 0;
  virtual void setText(const std::string text) = 0;
  virtual void setIndicator(const std::string imagePath) = 0;
  virtual void registerCallback(void (*callback)(void *), void *context) = 0;
  virtual void executeCallback() = 0;
};

} // namespace CapEngine
#endif // BUTTON_H

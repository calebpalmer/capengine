#ifndef BUTTON_H
#define BUTTON_H

#include "uiobject.h"
#include <string>

class Button : public UIObject {
 public:
  virtual ~Button() {}
  virtual std::string getText() const = 0;
  virtual void setText(const std::string text) = 0;
};

#endif // BUTTON_H

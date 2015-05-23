#ifndef UIOBJECT_H
#define UIOBJECT_H

#include "gameobject.h"

#include <memory>
#include <string>

class UIObject {
 public:
  virtual ~UIObject() {}
  virtual void update() = 0;
  virtual void render() = 0;
  virtual void setEnabled(bool enabled) = 0;
  virtual bool isEnabled() const = 0;
  virtual int getWidth() const = 0;
  virtual int getHeight() const = 0;
  virtual void setWidth(int width) = 0;
  virtual void setHeight(int height) = 0;
  virtual void setPosition(const CapEngine::Vector position) = 0;
};

#endif // UIOBJECT

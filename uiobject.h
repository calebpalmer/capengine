#ifndef UIOBJECT_H
#define UIOBJECT_H

#include <memory>
#include <string>

#include "vector.h"

namespace CapEngine {

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
    virtual void setPosition(const Vector position) = 0;
    virtual void setSelected(bool selected) = 0;
    virtual bool isSelected() const = 0;

  };

}
#endif // UIOBJECT

#ifndef UIOBJECT_H
#define UIOBJECT_H

#include <memory>
#include <string>

#include "vector.h"

namespace CapEngine
{

class UIObject
{
public:
  virtual ~UIObject() {}
  virtual void update() = 0;
  virtual void render() = 0;
  virtual void setEnabled(bool in_enabled) { m_enabled = in_enabled; };
  virtual bool isEnabled() { return m_enabled; };
  virtual int getWidth() const = 0;
  virtual int getHeight() const = 0;
  virtual void setWidth(int width) = 0;
  virtual void setHeight(int height) = 0;
  virtual void setPosition(const Vector position) = 0;
  virtual Vector getPosition() const = 0;
  virtual void setSelected(bool selected) = 0;
  virtual bool isSelected() const = 0;

protected:
  bool m_enabled = true;
};

} // namespace CapEngine
#endif // UIOBJECT

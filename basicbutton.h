#ifndef BASICBUTTON_H
#define BASICBUTTON_H

#include "CapEngine.h"
#include "button.h"

#include <string>

class BasicButton : public Button {
 private:
  BasicButton(const BasicButton&);
  BasicButton& operator=(const BasicButton&);

 public:
  BasicButton(std::string text, int fontSize, CapEngine::Vector position,  int width, int height);
  ~BasicButton();
  virtual void update();
  virtual void render();
  virtual void setEnabled(bool enabled = true);
  virtual bool isEnabled() const;
  virtual int getWidth() const;
  virtual int getHeight() const;
  virtual void setWidth(int width);
  virtual void setHeight(int height);
  virtual std::string getText() const;
  virtual void setText(const std::string text);

 protected:
  std::string m_text;
  int m_width;
  int m_height;
  bool m_selected;
  bool m_enabled;
  bool m_activated;
  CapEngine::Surface* m_pTextSurface;
  CapEngine::Vector m_position;

};
#endif // BASICBUTTON_H

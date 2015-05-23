#ifndef TEXTBUTTON_H
#define TEXTBUTTON_H

#include "CapEngine.h"
#include "button.h"

#include <string>

class TextButton : public Button, public IEventSubscriber {

 private:
  TextButton(const TextButton&);
  TextButton& operator=(const TextButton&);

 public:
  TextButton(std::string text, std::string font, int fontSize, CapEngine::Vector position);
  ~TextButton();
  void registerCallback(void (*callback)(void*), void* context);
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
  virtual void setPosition(const CapEngine::Vector position);
    // IEventSubscriber virtual
  virtual void receiveEvent(const SDL_Event* event, CapEngine::Time* time); 

 protected:
  std::string m_text;
  std::string m_font;
  int m_fontSize;
  int m_width;
  int m_height;
  bool m_selected;
  bool m_enabled;
  bool m_activated;
  CapEngine::Surface* m_pTextSurfaceInactive;
  CapEngine::Surface* m_pTextSurfaceActive;
  CapEngine::Vector m_position;
  void (*m_callback)(void *);
  void* m_context;

 protected:
  bool mouseInButton(CapEngine::Vector position);
};

#endif // TEXTBUTTON_H

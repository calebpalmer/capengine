#ifndef WIDGET_H
#define WIDGET_H

namespace CapEngine{

  class Widget{
  public:
    virtual void handleMouseMotionEvent(SDL_MouseMotionEvent event) {}
    virtual void handleMouseButtonEvent(SDL_MouseButtonEvent event) {}
    virtual void handleMouseWheelEvent(SDL_MouseWheelEvent event) {}
    virtual void handleKeyboardEvent(SDL_KeyboardEvent event) {}
    virtual void handleWindowEvent(SDL_WindowEvent event) {}
  };
}

#endif // WIDGET_H

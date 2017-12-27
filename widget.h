#ifndef WIDGET_H
#define WIDGET_H

#include <SDL2/SDL.h>

namespace CapEngine { namespace UI{

  class Widget{
  public:
		virtual ~Widget() {};

		virtual void setPosition(int x, int y) = 0;
		virtual void setSize(int width, int height) = 0;
		virtual void render() = 0;
		virtual void update(double ms) {};
		
    virtual void handleMouseMotionEvent(SDL_MouseMotionEvent event) {}
    virtual void handleMouseButtonEvent(SDL_MouseButtonEvent event) {}
    virtual void handleMouseWheelEvent(SDL_MouseWheelEvent event) {}
    virtual void handleKeyboardEvent(SDL_KeyboardEvent event) {}
    virtual void handleWindowEvent(SDL_WindowEvent event) {}
  };

	}
}

#endif // WIDGET_H

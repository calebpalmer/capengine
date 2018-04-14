#ifndef WIDGET_H
#define WIDGET_H

#include "VideoManager.h"

#include <memory>
#include <SDL2/SDL.h>

namespace CapEngine { namespace UI {

		class Widget{
		public:
			Widget() = default;
			Widget(Widget* pParentWidget);
			
			virtual ~Widget() = default;

			virtual void setPosition(int x, int y) = 0;
			virtual void setSize(int width, int height) = 0;
			virtual void render() = 0;
			virtual void update(double ms) {};
			virtual void setParent(Widget *pParent) { m_pParent = pParent; }
			virtual void setWindowId(Uint32 windowId) { m_windowId = windowId; }

			virtual void handleMouseMotionEvent(SDL_MouseMotionEvent /*event*/) {}
			virtual void handleMouseButtonEvent(SDL_MouseButtonEvent /*event*/) {}
			virtual void handleMouseWheelEvent(SDL_MouseWheelEvent /*event*/) {}
			virtual void handleKeyboardEvent(SDL_KeyboardEvent /*event*/) {}
			virtual void handleWindowEvent(SDL_WindowEvent /*event*/) {}

		protected:
			Uint32 m_windowId = VideoManager::kInvalidWindowId; //<! The owning window id
			Widget* m_pParent = nullptr; //<! The parent widget
		};

	}
}

#endif // WIDGET_H

#ifndef WIDGET_H
#define WIDGET_H

#include "VideoManager.h"
#include "uicommon.h"

#include <memory>
#include <optional>
#include <SDL2/SDL.h>

namespace CapEngine { namespace UI {

		class Widget{
		public:
			Widget() = default;
			Widget(Widget* pParentWidget);
			
			virtual ~Widget() = default;

			virtual SDL_Rect getPosition() const = 0;
			virtual void setPosition(int x, int y) = 0;
			virtual void setSize(int width, int height) = 0;
			virtual void render() = 0;
			virtual void update(double /*ms*/) {};
			virtual void setParent(Widget *pParent) { m_pParent = pParent; }
			virtual Uint32 getWindowId() const { return m_windowId; }
			virtual void setWindowId(Uint32 windowId) { m_windowId = windowId; }
			virtual std::vector<std::shared_ptr<Widget>> getChildren() { return {}; }
			virtual bool canFocus() const { return false; }
			virtual bool doFocus(bool, int, int, int, int) { return false; }
			virtual void setBorder(BorderStyle /*borderStyle*/, unsigned int /*borderWidth*/) {}

			virtual void handleMouseMotionEvent(SDL_MouseMotionEvent /*event*/) {}
			virtual void handleMouseButtonEvent(SDL_MouseButtonEvent /*event*/) {}
			virtual void handleMouseWheelEvent(SDL_MouseWheelEvent /*event*/) {}
			virtual void handleKeyboardEvent(SDL_KeyboardEvent /*event*/) {}
			virtual void handleWindowEvent(SDL_WindowEvent /*event*/) {}
			virtual void handleTextInputEvent(SDL_TextInputEvent /*event*/) {}

		protected:
			Uint32 m_windowId = VideoManager::kInvalidWindowId; //<! The owning window id
			Widget* m_pParent = nullptr; //<! The parent widget
		};

	}
}


/** 
 \fn Widget::getPosition
 \brief Gets the position of the widget.
 \return 
   \li The position of the widget.
*/

/** 
 \fn Widget::getChildren
 \brief Gets the child widgets of this widget.
 \return 
   \li The child widgets
*/


/** 
 \fn Widget::canFocus
 \brief Checks if widgets can take focus or not.
 \return 
   \li true if focusable; false otherwise.
*/


/** 
 \fn Widget::doFocus
 \brief handle focus/unfocus requests
 \param focus
   \li flag indicating if focus is given or removed
 \param downX
   \li The x location in the window where click ocurred.
 \param downY
   \li The y location in the window where click ovurred.
 \param upX
   \li The x location in the window where click ocurred.
 \param upY
   \li The y location in the window where click ovurred.

 \return 
   \li true if focus was received, false otherwise.
*/


/** 
 \fn Widget::getWindowId
 \brief Gets the window id of the widget.
 \return 
   \li The window id
*/


/** 
 \fn Widget::handleTextInputEvent
 \brief Handles Text Input events
 \param event
   \li The text input event tohandle.
*/


/**
 \fn Widget::setBorder
 \brief Sets the border on a widget.  This is an optionally implemented virtual function.
 \param borderStyle
   \li The border style to use.
 \param 
   \li The width of the border.
*/



#endif // WIDGET_H

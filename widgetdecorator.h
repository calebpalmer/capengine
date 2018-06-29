#ifndef CAPENGINE_WIDGETDECORATOR_H
#define CAPENGINE_WIDGETDECORATOR_H

#include "widget.h"

namespace CapEngine { namespace UI {

class WidgetDecorator : public Widget {
public:
	WidgetDecorator(std::shared_ptr<Widget> pWidget);

	virtual ~WidgetDecorator() = default;

	virtual void setPosition(int x, int y);
	virtual void setSize(int width, int height);
	virtual void render();
	virtual void update(double ms);
	virtual void setParent(Widget *pParent);
	virtual void setWindowId(Uint32 windowId);

	virtual void handleMouseMotionEvent(SDL_MouseMotionEvent /*event*/);
	virtual void handleMouseButtonEvent(SDL_MouseButtonEvent /*event*/);
	virtual void handleMouseWheelEvent(SDL_MouseWheelEvent /*event*/);
	virtual void handleKeyboardEvent(SDL_KeyboardEvent /*event*/);
	virtual void handleWindowEvent(SDL_WindowEvent /*event*/);

protected:
	std::shared_ptr<Widget> m_pWidget;
};
		
}}


#endif // CAPENGINE_WIDGETDECORATOR

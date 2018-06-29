#include "widgetdecorator.h"

namespace CapEngine { namespace UI {


//! Constructor
/** 
 \param pWidget
   \li The widget to decorate.
*/
WidgetDecorator::WidgetDecorator(std::shared_ptr<Widget> pWidget)
	: m_pWidget(pWidget)
{
}

//! \copydoc Widget::setPosition
void WidgetDecorator::setPosition(int x, int y){
	m_pWidget->setPosition(x, y);
}

//! \copydoc Widget::setSize
void WidgetDecorator::setSize(int width, int height){
	m_pWidget->setPosition(width, height);
}

//! \copydoc Widget::render
void WidgetDecorator::render(){
	m_pWidget->render();
}

//! \copydoc Widget::update
void WidgetDecorator::update(double ms){
	m_pWidget->update(ms);
}

//! \copydoc Widget::setParent
void WidgetDecorator::setParent(Widget *pParent){
	m_pWidget->setParent(pParent);
}

//! \copydoc Widget::setWindowId
void WidgetDecorator::setWindowId(Uint32 windowId){
	m_pWidget->setWindowId(windowId);
}

//! \copydoc Widget::handleMouseMotionEvent
void WidgetDecorator::handleMouseMotionEvent(SDL_MouseMotionEvent event){
	m_pWidget->handleMouseMotionEvent(event);
}

//! \copydoc Widget:handleMouseButtonEvent
void WidgetDecorator::handleMouseButtonEvent(SDL_MouseButtonEvent event){
	m_pWidget->handleMouseButtonEvent(event);
}

//! \copydoc Widget::handleMouseWheelEvent
void WidgetDecorator::handleMouseWheelEvent(SDL_MouseWheelEvent event){
	m_pWidget->handleMouseWheelEvent(event);
}

//! \copydoc Widget::handleKeyboardEvent
void WidgetDecorator::handleKeyboardEvent(SDL_KeyboardEvent event){
	m_pWidget->handleKeyboardEvent(event);
}

//! \copydoc Widget::handleWindowEvent
void WidgetDecorator::handleWindowEvent(SDL_WindowEvent event){
	m_pWidget->handleWindowEvent(event);
}

}}

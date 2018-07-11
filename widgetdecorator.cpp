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
	assert(m_pWidget != nullptr);
}

//! \copydoc Widget::setPosition
void WidgetDecorator::setPosition(int x, int y){
	assert(m_pWidget != nullptr);
	m_pWidget->setPosition(x, y);
}

//! \copydoc Widget::setSize
void WidgetDecorator::setSize(int width, int height){
	assert(m_pWidget != nullptr);	
	m_pWidget->setPosition(width, height);
}

//! \copydoc Widget::render
void WidgetDecorator::render(){
	assert(m_pWidget != nullptr);	
	m_pWidget->render();
}

//! \copydoc Widget::update
void WidgetDecorator::update(double ms){
	assert(m_pWidget != nullptr);	
	m_pWidget->update(ms);
}

//! \copydoc Widget::setParent
void WidgetDecorator::setParent(Widget *pParent){
	assert(m_pWidget != nullptr);	
	m_pWidget->setParent(pParent);
}

//! \copydoc Widget::setWindowId
void WidgetDecorator::setWindowId(Uint32 windowId){
	assert(m_pWidget != nullptr);	
	m_pWidget->setWindowId(windowId);
}

//! \copydoc Widget::handleMouseMotionEvent
void WidgetDecorator::handleMouseMotionEvent(SDL_MouseMotionEvent event){
	assert(m_pWidget != nullptr);	
	m_pWidget->handleMouseMotionEvent(event);
}

//! \copydoc Widget:handleMouseButtonEvent
void WidgetDecorator::handleMouseButtonEvent(SDL_MouseButtonEvent event){
	assert(m_pWidget != nullptr);	
	m_pWidget->handleMouseButtonEvent(event);
}

//! \copydoc Widget::handleMouseWheelEvent
void WidgetDecorator::handleMouseWheelEvent(SDL_MouseWheelEvent event){
	assert(m_pWidget != nullptr);	
	m_pWidget->handleMouseWheelEvent(event);
}

//! \copydoc Widget::handleKeyboardEvent
void WidgetDecorator::handleKeyboardEvent(SDL_KeyboardEvent event){
	assert(m_pWidget != nullptr);	
	m_pWidget->handleKeyboardEvent(event);
}

//! \copydoc Widget::handleWindowEvent
void WidgetDecorator::handleWindowEvent(SDL_WindowEvent event){
	assert(m_pWidget != nullptr);	
	m_pWidget->handleWindowEvent(event);
}

//! \copydoc Widget::getChildren
std::vector<std::shared_ptr<Widget>> WidgetDecorator::getChildren() {
	assert(m_pWidget != nullptr);	
	return m_pWidget->getChildren();
}

//! \copydoc Widget::canFocus
bool WidgetDecorator::canFocus() const {
	assert(m_pWidget != nullptr);
	return m_pWidget->canFocus();
}

//! \copydoc Widget::getPosition
SDL_Rect WidgetDecorator::getPosition() const {
	assert(m_pWidget != nullptr);
	return m_pWidget->getPosition();
}

}}

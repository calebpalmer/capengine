#include "absolutelayout.h"

#include "CapEngineException.h"

namespace CapEngine { namespace UI {

//! Constructor
/**
* \param pWindow - The owning Window
*/
AbsoluteLayout::AbsoluteLayout(Widget *pParent) :
	Widget(pParent) {}

//! Add a widget to the layout
/**
* \param pWidget - The widget to add
* \param x - the relative x location of the widget in the layout
* \param y - the relative y location of the widget in the layout
* \param width - the width of the widget
* \param height - the height of the widget
*/
void AbsoluteLayout::addWidget(std::shared_ptr<Widget> pWidget, int x, int y, int width, int height){
	CAP_THROW_ASSERT(pWidget != nullptr, "Can't add null widget");
	CAP_THROW_ASSERT(x >= 0 && y >= 0, "Invalid X and Y");

	WidgetLocation widgetLocation = {pWidget, x, y, width, height};
	m_widgets.push_back(widgetLocation);

	pWidget->setPosition(m_x + x, m_y + y);

	// make sure location doesn't extend past bounds of layout
	int newWidth = widgetLocation.x + widgetLocation.width <= m_x + m_width ?
		width : m_width - x;

	int newHeight = widgetLocation.y + widgetLocation.height <= m_y + m_height ?
		height : m_height - y;
		
	pWidget->setSize(newWidth, newHeight);
}

//! Removes a widget from the layout
/**
* \param pWidget - the widget to remove
*/
void AbsoluteLayout::removeWidget(std::shared_ptr<Widget> pWidget){
		std::remove_if(m_widgets.begin(), m_widgets.end(),
							 [&pWidget](const WidgetLocation &widgetLocation) {
								 return widgetLocation.pWidget == pWidget;
							 });

}

//! @copydoc Widget::render()
void AbsoluteLayout::render(){
	for(auto && widget : m_widgets){
		widget.pWidget->render();
	}
}

//! @copydoc Widget::setPosition()
void AbsoluteLayout::setPosition(int x, int y){
	m_x = x;
	m_y = y;
}

//! @copydoc Widget::setSize()
void AbsoluteLayout::setSize(int width, int height){
	m_width = width;
	m_height = height;
}

//! @copydoc Widget::handleMouseMotionEvent()
void AbsoluteLayout::handleMouseMotionEvent(SDL_MouseMotionEvent event) {
	for(auto && widgetLocation : m_widgets){
		widgetLocation.pWidget->handleMouseMotionEvent(event);
	}
}

//! @copydoc Widget::handleMouseButtonEvent()
void AbsoluteLayout::handleMouseButtonEvent(SDL_MouseButtonEvent event) {
	for(auto && widgetLocation : m_widgets){
		widgetLocation.pWidget->handleMouseButtonEvent(event);
	}
}

//! @copydoc Widget::handleMouseWheelEvent()
void AbsoluteLayout::handleMouseWheelEvent(SDL_MouseWheelEvent event) {
	for(auto && widgetLocation : m_widgets){
		widgetLocation.pWidget->handleMouseWheelEvent(event);
	}
}

//! @copydoc Widget::handleKeyboardEvent()
void AbsoluteLayout::handleKeyboardEvent(SDL_KeyboardEvent event) {
	for(auto && widgetLocation : m_widgets){
		widgetLocation.pWidget->handleKeyboardEvent(event);
	}
}  

//! @copydoc Widget::handleWindowEvent()
void AbsoluteLayout::handleWindowEvent(SDL_WindowEvent event) {
	for(auto && widgetLocation : m_widgets){
		widgetLocation.pWidget->handleWindowEvent(event);
	}
}


}} // namespace CapEngine::UI

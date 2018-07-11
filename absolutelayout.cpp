#include "absolutelayout.h"
#include "utils.h"

#include "CapEngineException.h"

#include <algorithm>

namespace CapEngine { namespace UI {

//! Constructor
AbsoluteLayout::AbsoluteLayout() {}


//! Creates an AbsoluteLaout
/** 
 \param pParent The parent widget
 \return The created AbsoluteLayout object
*/
std::shared_ptr<AbsoluteLayout> AbsoluteLayout::create(){
	std::shared_ptr<AbsoluteLayout> pLayout(new AbsoluteLayout());
	return pLayout;
}

//! Add a widget to the layout
/**
* \param pWidget - The widget to add
* \param x - the relative x location of the widget in the layout
* \param y - the relative y location of the widget in the layout
* \param width - the width of the widget
* \param height - the height of the widget
*/
void AbsoluteLayout::addWidget(std::shared_ptr<Widget> pWidget, int x, int y, int width, int height, Unit widthUnit, Unit heightUnit){
	CAP_THROW_ASSERT(pWidget != nullptr, "Can't add null widget");
	CAP_THROW_ASSERT(x >= 0 && y >= 0, "Invalid X and Y");

	WidgetLocation widgetLocation = {pWidget, x, y, width, height, widthUnit, heightUnit};
	m_widgets.push_back(widgetLocation);

	pWidget->setParent(this);
	pWidget->setWindowId(this->m_windowId);

// 	pWidget->setPosition(m_x + x, m_y + y);

// 	// make sure location doesn't extend past bounds of layout
// 	int newWidth = widgetLocation.x + widgetLocation.width <= m_x + m_width ?
// 		width : m_width - x;

// 	int newHeight = widgetLocation.y + widgetLocation.height <= m_y + m_height ?
// 		height : m_height - y;

// 	pWidget->setSize(newWidth, newHeight);
	m_updateWidgets = true;
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
	if(m_updateWidgets)
		updateWidgetPositions();
	
	for(auto && widget : m_widgets){
		widget.pWidget->render();
	}
}

//! @copydoc Widget::setPosition()
void AbsoluteLayout::setPosition(int x, int y){
	m_x = x;
	m_y = y;
	m_updateWidgets = true;
}

//! @copydoc Widget::setSize()
void AbsoluteLayout::setSize(int width, int height){
	m_width = width;
	m_height = height;
	m_updateWidgets = true;
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

//! \copydoc Widget::setWindowId
void AbsoluteLayout::setWindowId(Uint32 windowId){
	Widget::setWindowId(windowId);

	for(auto && widgetLocation : m_widgets){
		assert(widgetLocation.pWidget);
		widgetLocation.pWidget->setWindowId(windowId);
	}
}


//! update widget positions
void AbsoluteLayout::updateWidgetPositions(){

	for(auto && widgetLocation : m_widgets){
		auto pWidget = widgetLocation.pWidget;
		assert(pWidget);

		SDL_Rect srcRect = { widgetLocation.x, widgetLocation.y, widgetLocation.width, widgetLocation.height };

		if(widgetLocation.widthUnit == Unit::Percentage)
			srcRect.w = m_x + m_width - srcRect.x;

		if(widgetLocation.heightUnit == Unit::Percentage)
			srcRect.h = m_y + m_height - srcRect.y;
		
		SDL_Rect dstRect = { m_x, m_y, m_width, m_height };

		std::optional<SDL_Rect> clippedRect = clipRect(srcRect, dstRect);
		assert(clippedRect != std::nullopt);

		pWidget->setPosition(clippedRect->x, clippedRect->y);		
		pWidget->setSize(clippedRect->w, clippedRect->h);
	}

	m_updateWidgets = false;
}

//! Widget::getChildren
std::vector<std::shared_ptr<Widget>> AbsoluteLayout::getChildren() {
	std::vector<std::shared_ptr<Widget>> widgets(m_widgets.size());
	std::transform(m_widgets.begin(), m_widgets.end(), widgets.begin(),
								 [&widgets](const WidgetLocation &widgetLocation) { return widgetLocation.pWidget; } );

	return widgets;
}

//! Widget::getPosition
SDL_Rect AbsoluteLayout::getPosition() const {
	return { m_x, m_y, m_width, m_height };
}

}} // namespace CapEngine::UI

#include "horizontallayout.h"

namespace CapEngine { namespace UI {

//! Creates a HorizontalLayout
/** 
 \return the HorizontalLayout
*/
std::shared_ptr<HorizontalLayout> HorizontalLayout::create(){
	return std::shared_ptr<HorizontalLayout>(new HorizontalLayout);
}

//! \copydoc Widget::setPosition
void HorizontalLayout::setPosition(int x, int y){
	m_rect.x = x;
	m_rect.y = y;

	m_updateChildren = true;
}

//! \copydoc Widget::setSize
void HorizontalLayout::setSize(int width, int height){
	m_rect.w = width;
	m_rect.h = height;

	m_updateChildren = true;
}

//! \copydoc Widget::render
void HorizontalLayout::render(){
	if(m_updateChildren)
		this->updateChildren();
	
	for(auto && pWidget : m_widgets){
		CAP_THROW_NULL(pWidget, "Widget is null");
		pWidget->render();
	}
}

//! \copydoc Widget::update
void HorizontalLayout::update(double ms){
	for(auto && pWidget : m_widgets){
		CAP_THROW_NULL(pWidget, "Widget is null");
			pWidget->update(ms);
	}
}

//! \copydoc Widget::setWindowId
void HorizontalLayout::setWindowId(Uint32 windowId){
	m_windowId = windowId;

	for(auto && pWidget : m_widgets){
		CAP_THROW_NULL(pWidget, "Widget is null");
		pWidget->setWindowId(windowId);
	}
}

//! Adds a widget to the layout
/** 
 \param pWidget - The widget to add.
*/
void HorizontalLayout::addWidget(std::shared_ptr<Widget> pWidget){
	CAP_THROW_NULL(pWidget, "The widget is null");
	m_widgets.push_back(pWidget);

	pWidget->setParent(this);
	pWidget->setWindowId(this->m_windowId);

	m_updateChildren = true;
}


//! update size and position of children
void HorizontalLayout::updateChildren(){
	int widgetWidth = m_rect.w / m_widgets.size();
	int widgetHeight = m_rect.h;

	for(size_t i = 0; i < m_widgets.size(); i++){
		CAP_THROW_NULL(m_widgets[i], "Widget is null");
		m_widgets[i]->setSize(widgetWidth, widgetHeight);
		m_widgets[i]->setPosition(m_rect.x + i * widgetWidth, m_rect.y);
	}

	m_updateChildren = false;
}

//! \copydoc Widget::handleMouseMotionEvent
void HorizontalLayout::handleMouseMotionEvent(SDL_MouseMotionEvent event){
	for(auto && pWidget : m_widgets){
		pWidget->handleMouseMotionEvent(event);
	}
}

//! \copydoc Widget::handleMouseButtonEvent
void HorizontalLayout::handleMouseButtonEvent(SDL_MouseButtonEvent event){
	for(auto && pWidget : m_widgets){
		pWidget->handleMouseButtonEvent(event);
	}
}

//! \copydoc Widget::handleMouseWheelEvent
void HorizontalLayout::handleMouseWheelEvent(SDL_MouseWheelEvent event){
	for(auto && pWidget : m_widgets){
		pWidget->handleMouseWheelEvent(event);
	}
}

//! \copydoc Widget::handleKeyboardEvent
void HorizontalLayout::handleKeyboardEvent(SDL_KeyboardEvent event){
	for(auto && pWidget : m_widgets){
		pWidget->handleKeyboardEvent(event);
	}
}

//! \copydoc Widget::handleWindowEvent
void HorizontalLayout::handleWindowEvent(SDL_WindowEvent event){
	for(auto && pWidget : m_widgets){
		pWidget->handleWindowEvent(event);
	}
}

}}

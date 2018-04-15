#include "widgetstate.h"

#include "game_management.h"
#include "locator.h"

#include <functional>

namespace CapEngine { namespace UI {


//! Constructor
/** 
 \param onLoadFunctor - Code to call when loading the GameState
 \param onDestroyFunctor - Function to call when destroying the GameState
*/
WidgetState::WidgetState(std::function<bool(WidgetState& widgetState)> onLoadFunctor,
						std::function<bool(WidgetState& widgetState)> onDestroyFunctor) :
	m_onLoadFunctor(onLoadFunctor), m_onDestroyFunctor(onDestroyFunctor) {

	// create a register a control stack with the locator
	m_pUiControls = std::make_shared<std::vector<std::shared_ptr<UI::Control>>>();

	// register our event handlers with eventsubscriber signals
	assert(Locator::eventSubscriber != nullptr);

	m_keyboardEventConnection =
		Locator::eventSubscriber->m_keyboardEventSignal.connect(std::bind(&WidgetState::handleKeyboardEvent, this, std::placeholders::_1));

	m_mouseButtonEventConnection =
		Locator::eventSubscriber->m_mouseButtonEventSignal.connect(std::bind(&WidgetState::handleMouseButtonEvent, this, std::placeholders::_1));

	m_mouseMotionEventConnection =
		Locator::eventSubscriber->m_mouseMotionEventSignal.connect(std::bind(&WidgetState::handleMouseMotionEvent, this, std::placeholders::_1));
	
	m_mouseWheelEventConnection =
		Locator::eventSubscriber->m_mouseWheelEventSignal.connect(std::bind(&WidgetState::handleMouseWheelEvent, this, std::placeholders::_1));

	m_windowEventConnection =
		Locator::eventSubscriber->m_windowEventSignal.connect(std::bind(&WidgetState::handleWindowEvent, this, std::placeholders::_1));
}

//! creates a WidgetState
/** 
 \param onLoadFunctor - Code to call when loading the GameState
 \param onDestroyFunctor - Function to call when destroying the GameState
 \return - The WidgetState
*/
std::shared_ptr<WidgetState> WidgetState::create(std::function<bool(WidgetState& widgetState)> onLoadFunctor,
																		std::function<bool(WidgetState& widgetState)> onDestroyFunctor)
{
	std::shared_ptr<WidgetState>pWidgetState(new WidgetState(onLoadFunctor, onDestroyFunctor));

	Locator::insert(kWidgetStateLocatorId, pWidgetState);
	return pWidgetState;
}


//! @copydoc GameState::onLoad()
bool WidgetState::onLoad(){
	return m_onLoadFunctor(*this);
}

//! @copydoc GameState::onDestroy()
bool WidgetState::onDestroy(){
	return m_onDestroyFunctor(*this);
}

//! @copydoc GameState::render()
void WidgetState::render(){
	for(auto && pWindow : m_pWindows){
		CAP_THROW_NULL(pWindow, "Window is null");
		pWindow->render();
	}

	assert(m_pUiControls != nullptr);
	if(m_pUiControls->size() > 0){
		auto pControl = m_pUiControls->back();
		assert(pControl != nullptr);
		pControl->render();
	}
	
	m_postRenderSignal(*this);
}

//! @copydoc GameState::update()
void WidgetState::update(double ms){
	if(m_pQueuedUiControl != nullptr){

		assert(m_pUiControls != nullptr);
		m_pUiControls->push_back(m_pQueuedUiControl);

		m_pQueuedUiControl.reset();
	}
	
	for(auto && pWindow : m_pWindows){
		CAP_THROW_NULL(pWindow, "Window is null");
		pWindow->update(ms);
	}
}

//! create a window
/** 
 \param name - The name of the window
 \param width - The width of the window
 \param height - The height of the window
 \return The WindowWidget
*/
std::shared_ptr<WindowWidget> WidgetState::createWindow(const std::string &name, int width, int height, bool resizable){
	// not using CapEngine::createWindow because we want to handle receiving the event signals here
	// and passing them to any child windows
	std::shared_ptr<WindowWidget> pWindowWidget = WindowWidget::create(name, width, height, resizable);
	CAP_THROW_NULL(pWindowWidget, "Error creating window.  Window is null");
	m_pWindows.push_back(pWindowWidget);

	return pWindowWidget;
}


//! registers a handler for post render signals
/** 
 \param slot
 \return - The scoped_connection for the signal/slot
*/
boost::signals2::scoped_connection WidgetState::connectPostRenderSignal(std::function<void (WidgetState &)> slot){
	return m_postRenderSignal.connect(slot);
}

//! \copydoc Widget::handleMouseMotionEvent
void WidgetState::handleMouseMotionEvent(SDL_MouseMotionEvent event){
	// send event to windows
	for(auto&& pWindow : m_pWindows){
		assert(pWindow != nullptr);
		pWindow->handleMouseMotionEvent(event);
	}

	// send event to active control
	assert(m_pUiControls != nullptr);
	if(m_pUiControls->size() > 0){
		auto pControl = m_pUiControls->back();
		assert(pControl != nullptr);
		pControl->handleMouseMotionEvent(event);
	}
}

//! \copydoc Widget::handleMouseButtonEvent
void WidgetState::handleMouseButtonEvent(SDL_MouseButtonEvent event){
	// send event to windows
	for(auto&& pWindow : m_pWindows){
		assert(pWindow != nullptr);
		pWindow->handleMouseButtonEvent(event);
	}

	// send event to active control
	assert(m_pUiControls != nullptr);
	if(m_pUiControls->size() > 0){
		auto pControl = m_pUiControls->back();
		assert(pControl != nullptr);
		pControl->handleMouseButtonEvent(event);
	}

}

//! \copydoc Widget::handleMouseWheelEvent
void WidgetState::handleMouseWheelEvent(SDL_MouseWheelEvent event){
	// send event to windows
	for(auto&& pWindow : m_pWindows){
		assert(pWindow != nullptr);
		pWindow->handleMouseWheelEvent(event);
	}

		// send event to active control
	assert(m_pUiControls != nullptr);
	if(m_pUiControls->size() > 0){
		auto pControl = m_pUiControls->back();
		assert(pControl != nullptr);
		pControl->handleMouseWheelEvent(event);
	}

}

//! \copydoc Widget::handleKeyboardEvent
void WidgetState::handleKeyboardEvent(SDL_KeyboardEvent event){
	// send event to windows
	for(auto&& pWindow : m_pWindows){
		assert(pWindow != nullptr);
		pWindow->handleKeyboardEvent(event);
	}

	// send event to active control
	assert(m_pUiControls != nullptr);
	if(m_pUiControls->size() > 0){
		auto pControl = m_pUiControls->back();
		assert(pControl != nullptr);
		pControl->handleKeyboardEvent(event);
	}

}

//! \copydoc Widget::handleWindowEvent
void WidgetState::handleWindowEvent(SDL_WindowEvent event){
	// send event to windows
	for(auto&& pWindow : m_pWindows){
		assert(pWindow != nullptr);
		pWindow->handleWindowEvent(event);
	}

	// send event to active control
	assert(m_pUiControls != nullptr);
	if(m_pUiControls->size() > 0){
		auto pControl = m_pUiControls->back();
		assert(pControl != nullptr);
		pControl->handleWindowEvent(event);
	}

}

//! Adds a control to the control.
/** 
 \param pControl - The control to add.
*/
void WidgetState::addControl(std::shared_ptr<UI::Control> pControl){
	assert(pControl != nullptr);
	m_pQueuedUiControl = pControl;
}

//! Removes the current control and returns it
/** 
 \return - The current control.  boost::none if there isn't one.
*/
boost::optional<std::shared_ptr<UI::Control>> WidgetState::popControl(){
	assert(m_pUiControls != nullptr);

	auto pControl = this->peekControl();
	m_pUiControls->pop_back();
	return pControl;
}

//! Gets the active control
/** 
 \return - The current control.  boost::none if there isn't one.
*/
boost::optional<std::shared_ptr<UI::Control>> WidgetState::peekControl(){
	assert(m_pUiControls != nullptr);

	if(m_pUiControls->size() > 0){
		auto pControl = m_pUiControls->back();
		assert(pControl != nullptr);

		return pControl;
	}

	else{
		return boost::none;
	}
}


}} // namespace CapEngine::UI

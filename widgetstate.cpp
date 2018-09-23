#include "widgetstate.h"

#include "game_management.h"
#include "locator.h"
#include "dialogstate.h"

#include <functional>
#include <algorithm>

namespace CapEngine { namespace UI {

namespace {


//! Removes controls that are finished.
/** 
 \param controls
   The list of controls.
*/
void checkForFinishedControls(std::vector<std::shared_ptr<UI::Control>> &controls){
	while(controls.size() > 0){
		auto pControl = controls.back();
		assert(pControl != nullptr);

		if(!pControl->isHandled())
			break;

		controls.pop_back();
	}
}

}


//! default Constructor
WidgetState::WidgetState()
	: WidgetState([](WidgetState&) {return true;}, [](WidgetState&) {return true;}) {}

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

	m_textInputEventConnection =
		Locator::eventSubscriber->m_textInputEventSignal.connect(std::bind(&WidgetState::handleTextInputEvent, this, std::placeholders::_1));
	

	// disable default quit events
	CapEngine::setDefaultQuitEvents(false);
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
void WidgetState::render()
{
	try{
		for(auto && pWindow : m_windows){
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

	catch(const std::exception &e){
		CAP_LOG_EXCEPTION(Locator::logger, e, Logger::CERROR);
	}
}

//! @copydoc GameState::update()
void WidgetState::update(double ms){
	try{
		if(m_pQueuedUiControl != boost::none){

			assert(m_pUiControls != nullptr);
			m_pUiControls->push_back(*m_pQueuedUiControl);

			m_pQueuedUiControl = boost::none;
		}
	
		for(auto && pWindow : m_windows){
			CAP_THROW_NULL(pWindow, "Window is null");
			pWindow->update(ms);
		}
	}

	catch(const std::exception &e){
		CAP_LOG_EXCEPTION(Locator::logger, e, Logger::CERROR);
	}
}

//! create a window
/** 
		\param name - The name of the window
		\param width - The width of the window
		\param height - The height of the window
		\param fullScreen
		  \li Flag indicating if window is created full screen.
		\return The WindowWidget
*/
std::shared_ptr<WindowWidget> WidgetState::createWindow(const std::string &name, int width, int height, bool resizable){
	// not using CapEngine::createWindow because we want to handle receiving the event signals here
	// and passing them to any child windows
	std::shared_ptr<WindowWidget> pWindowWidget = WindowWidget::create(name, width, height, resizable);
	CAP_THROW_NULL(pWindowWidget, "Error creating window.  Window is null");
	m_windows.push_back(pWindowWidget);

	m_windowConnections.push_back(pWindowWidget->registerWindowClosedSignal(std::bind(&WidgetState::handleWindowCloseSignal, this,  std::placeholders::_1)));

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
	try {
		// send event to windows
		for(auto&& pWindow : m_windows){
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
	
	catch(const std::exception &e){
		CAP_LOG_EXCEPTION(Locator::logger, e, Logger::CERROR);
	}
}

//! \copydoc Widget::handleMouseButtonEvent
void WidgetState::handleMouseButtonEvent(SDL_MouseButtonEvent event){
	try{
		if(event.type == SDL_MOUSEBUTTONDOWN){
			m_lastMouseDownPosition.first = event.x;
			m_lastMouseDownPosition.second = event.y;
			m_lastMouseDownWindowId = event.windowID;
		}

		else if(event.type == SDL_MOUSEBUTTONUP){
			// UI focus change handling
			if(event.windowID == m_lastMouseDownWindowId){
				auto maybeWindow =
					std::find_if(m_windows.begin(), m_windows.end(),
											 [this](std::shared_ptr<WindowWidget> pWindow)
											 {
												 return pWindow->getWindowId() == this->m_lastMouseDownWindowId;
											 });

				if(maybeWindow != m_windows.end()){
					bool handled = this->handleMouseFocusChange(*maybeWindow, event.x, event.y);
					
					if(!handled && m_pFocusedWidget != nullptr){
						m_pFocusedWidget->doFocus(false, -1, -1, -1, -1);
						m_pFocusedWidget = nullptr;
					}
				}
			}
		}
	
		// send event to windows
		for(auto&& pWindow : m_windows){
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

	catch(const std::exception &e){
		CAP_LOG_EXCEPTION(Locator::logger, e, Logger::CERROR);
	}


}

//! \copydoc Widget::handleMouseWheelEvent
void WidgetState::handleMouseWheelEvent(SDL_MouseWheelEvent event){
	try {
		// send event to windows
		for(auto&& pWindow : m_windows){
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

	catch(const std::exception &e){
		CAP_LOG_EXCEPTION(Locator::logger, e, Logger::CERROR);
	}


}

//! \copydoc Widget::handleKeyboardEvent
void WidgetState::handleKeyboardEvent(SDL_KeyboardEvent event){
	try{
		// send event to windows
		for(auto&& pWindow : m_windows){
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

	catch(const std::exception &e){
		CAP_LOG_EXCEPTION(Locator::logger, e, Logger::CERROR);
	}
}

//! \copydoc Widget::handleWindowevent
void WidgetState::handleWindowEvent(SDL_WindowEvent event){
	try{
		// send event to windows
		for(auto&& pWindow : m_windows){
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

	catch(const std::exception &e){
		CAP_LOG_EXCEPTION(Locator::logger, e, Logger::CERROR);
	}
}

//! \copydoc Widget::handleWindowevent
void WidgetState::handleTextInputEvent(SDL_TextInputEvent event){
	try {
		// send event to windows
		for(auto&& pWindow : m_windows){
			assert(pWindow != nullptr);
			pWindow->handleTextInputEvent(event);
		}

		// send event to active control
		assert(m_pUiControls != nullptr);
		if(m_pUiControls->size() > 0){
			auto pControl = m_pUiControls->back();
			assert(pControl != nullptr);
			pControl->handleTextInputEvent(event);
		}
	}

	catch(const std::exception &e){
		CAP_LOG_EXCEPTION(Locator::logger, e, Logger::CERROR);
	}

}

//! Adds a control to the control.
/** 
		\param pControl - The control to add.
*/
void WidgetState::addControl(std::shared_ptr<UI::Control> pControl){
	// TODO What should it do if there is a queued control already?
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
	checkForFinishedControls(*m_pUiControls);

	if(m_pUiControls->size() > 0){
		auto pControl = m_pUiControls->back();
		assert(pControl != nullptr);

		return pControl;
	}

	else{
		return boost::none;
	}
}

//! Shows a yes/no modal dialog
/** 
		\param msg
		The message to display
		\param callback
		\The callback function to call.
*/
void WidgetState::showOkCancelDialog(const std::string &msg, std::function<void(bool)> callback){
	std::shared_ptr<DialogState> pDialogState = DialogState::create(msg, callback);
	CapEngine::pushState(pDialogState);
}


//! receive window close signals
void WidgetState::handleWindowCloseSignal(WindowWidget* pWindowWidget){
	auto window = std::find_if(m_windows.begin(), m_windows.end(), [&](const decltype(m_windows)::value_type &pWindow){
		return pWindow.get() == pWindowWidget;
	});

	if(window != m_windows.end())
		m_windows.erase(window);

	if(m_windows.size() == 0){
		CapEngine::popState();
	}
}


//! Recursively check for focus change based on mouse click
/** 
 \param widget
   \li The window where the moust was clicked.
 \param x
   \li The x position of the mouse click.
 \param y
   \li The y position of the mouse click
*/
bool WidgetState::handleMouseFocusChange(std::shared_ptr<Widget> widget, int x, int y){
	try {
		assert(widget != nullptr);
	
		bool handled = false;
		if(widget->canFocus()){
			handled = widget->doFocus(true, m_lastMouseDownPosition.first, m_lastMouseDownPosition.second, x, y);
			if(handled){
				if(m_pFocusedWidget != nullptr && m_pFocusedWidget.get() != widget.get()){
					m_pFocusedWidget->doFocus(false, m_lastMouseDownPosition.first, m_lastMouseDownPosition.second, x, y);
				}

				m_pFocusedWidget = widget;
			}
		}

		if(!handled){
			for(auto && pWidget : widget->getChildren()){
				handled = handleMouseFocusChange(pWidget, x, y);

				if(handled)
					break;
			}
		}

		return handled;

	}

	catch(const std::exception &e){
		CAP_LOG_EXCEPTION(Locator::logger, e, Logger::CERROR);
		return false;
	}
}

}} // namespace CapEngine

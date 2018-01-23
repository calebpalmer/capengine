#include "windowwidget.h"

#include "VideoManager.h"
#include "locator.h"

namespace CapEngine { namespace UI {

//! WindowWidget Constructor
/**
* \param name The name of the Window.
* \param width The width of the Window.
* \param height The height of the Window.
* \param resizable Flag indicating if the window is resizable.
*/
WindowWidget::WindowWidget(const std::string &name, int width, int height, bool resizable)
	: m_windowName(name), m_width(width), m_height(height), m_resizable(resizable) { }

//! Show the window
void WindowWidget::show(){

  WindowParams windowParams = {
    m_windowName,
		m_width,
    m_height,
    32, // bpp,
    false, // fullscreen,
    false, // opengl
    m_resizable, // resizable
    m_windowName
  };

	CAP_THROW_ASSERT(Locator::videoManager != nullptr, "VideoManager is null");
	m_windowId = Locator::videoManager->createNewWindow(windowParams);

	Locator::videoManager->setBackgroundColour(m_backgroundColour);
}

//! Close the window
void WindowWidget::close(){
	CAP_THROW_ASSERT(Locator::videoManager != nullptr, "VideoManager is null");
	Locator::videoManager->closeWindow(m_windowId);
	m_windowId = VideoManager::kInvalidWindowId;
}

//! Destructor
WindowWidget::~WindowWidget(){
	if(m_windowId != VideoManager::kInvalidWindowId){
		try{
			this->close();
		}

		catch(const boost::exception& e){
			// print error
			std::cerr << boost::diagnostic_information(e) << std::endl;
		}
		catch(...){
			std::cerr << "Caught unknown exception in ~WindowWidget()" << std::endl;
		}
	}
}

//! Set the layout used for laying out child widgets
/**
* \param pLaout The Layout widget
*/
void WindowWidget::setLayout(std::shared_ptr<Widget> pLayout){
	m_pLayout = pLayout;
	m_pLayout->setPosition(0, 0);
	m_pLayout->setSize(m_width, m_height);
}


//! @copydoc Widget::setPosition()
void WindowWidget::setPosition(int x, int y){
	m_x = x;
	m_y = y;

	// if the  window has been created, move it
	if(m_windowId != VideoManager::kInvalidWindowId){
		CAP_THROW_ASSERT(Locator::videoManager != nullptr, "VideoManager is null");
		Locator::videoManager->setWindowPosition(m_windowId, m_x, m_y);
	}
}

//! @copydoc Widget::setSize()
void WindowWidget::setSize(int width, int height){
	m_width  = 0;
	m_height = 0;

	// if the  window has already been created, resize it
	if(m_windowId != VideoManager::kInvalidWindowId){
		CAP_THROW_ASSERT(Locator::videoManager != nullptr, "VideoManager is null");
		Locator::videoManager->setWindowSize(m_windowId, m_x, m_y);
	}
	
}

//! @copydoc Widget::render()
void WindowWidget::render(){
	// clear the screen
	CAP_THROW_NULL(Locator::videoManager, "VideoManager is null");
	CAP_THROW_ASSERT(m_windowId != VideoManager::kInvalidWindowId, "WindowId is invalid");
	
	if(m_pLayout)
		m_pLayout->render();
}

//! @copydoc Widget::handleMouseMotionEvent()
void WindowWidget::handleMouseMotionEvent(SDL_MouseMotionEvent event) {
	m_pLayout->handleMouseMotionEvent(event);
}

//! @copydoc Widget::handleMouseButtonEvent()
void WindowWidget::handleMouseButtonEvent(SDL_MouseButtonEvent event) {
	m_pLayout->handleMouseButtonEvent(event);
}

//! @copydoc Widget::handleMouseWheelEvent()
void WindowWidget::handleMouseWheelEvent(SDL_MouseWheelEvent event) {
	m_pLayout->handleMouseWheelEvent(event);
}

//! @copydoc Widget::handleKeyboardEvent()
void WindowWidget::handleKeyboardEvent(SDL_KeyboardEvent event) {
	m_pLayout->handleKeyboardEvent(event);
}  

//! @copydoc Widget::handleWindowEvent()
void WindowWidget::handleWindowEvent(SDL_WindowEvent event) {
	m_pLayout->handleWindowEvent(event);
}

//! Register handlers with event signals
/** 
* \param eventSubscriber - The EventSubscriber receiving the signals to subscribe to
*/
void WindowWidget::registerSignals(EventSubscriber &eventSubscriber){
	m_handleMouseMotionConnection =
		eventSubscriber.m_mouseMotionEventSignal.connect(std::bind(&WindowWidget::handleMouseMotionEvent, this, std::placeholders::_1));

	m_handleMouseButtonConnection =
		eventSubscriber.m_mouseButtonEventSignal.connect(std::bind(&WindowWidget::handleMouseButtonEvent, this, std::placeholders::_1));

	m_handleMouseWheelConnection =
		eventSubscriber.m_mouseWheelEventSignal.connect(std::bind(&WindowWidget::handleMouseWheelEvent, this, std::placeholders::_1));

	m_handleKeyboardConnection =
		eventSubscriber.m_keyboardEventSignal.connect(std::bind(&WindowWidget::handleKeyboardEvent, this, std::placeholders::_1));

	m_handleWindowConnection =
		eventSubscriber.m_windowEventSignal.connect(std::bind(&WindowWidget::handleWindowEvent, this, std::placeholders::_1));
}

}} // namespace CapEngine::UI

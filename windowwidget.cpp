#include "windowwidget.h"

#include "VideoManager.h"
#include "locator.h"
#include "game_management.h"
#include "dialogstate.h"

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


//! Creates a WindowWidget
/** 
 \param name The name of the Window.
 \param width The width of the Window.
 \param height The height of the Window.
 \param resizable Flag indicating if the window is resizable.
 \return The WindowWidget
*/
std::shared_ptr<WindowWidget> WindowWidget::create(const std::string &name,
																									 int width,
																									 int height,
																									 bool resizable)
{
	std::shared_ptr<WindowWidget> pWindowWidget(new WindowWidget(name, width,height, resizable));
	return pWindowWidget;
}

//! Show the window
void WindowWidget::show(){

  WindowParams windowParams = {
    m_windowName,
		m_width,
    m_height,
    32, // bpp,
    m_fullScreen, // fullscreen,
    false, // opengl
    m_resizable, // resizable
		m_maximized,
    m_windowName
  };

	CAP_THROW_ASSERT(Locator::videoManager != nullptr, "VideoManager is null");
	m_windowId = Locator::videoManager->createNewWindow(windowParams);

	Locator::videoManager->setBackgroundColour(m_backgroundColour);
	m_shown = true;
}

//! Sets if window should be maximized or not.
/** 
 \param maximized
   \li Flag indicating if window should be maximized.
*/
void WindowWidget::setMaximized(bool maximized){
	m_maximized = maximized;

	if(m_shown && m_resizable && m_maximized){
		assert(Locator::videoManager != nullptr);
		assert(m_windowId != VideoManager::kInvalidWindowId);
		
		Locator::videoManager->maximizeWindow(m_windowId);
	}
}

//! Set the window to full screen.
/** 
 \param fullScreen
   \li flag indicating if it should be full screen.
 \param 
 \return 
*/
void WindowWidget::setFullScreen(bool fullScreen){
	m_fullScreen = fullScreen;

	if(m_shown && m_resizable){
		assert(Locator::videoManager != nullptr);
		assert(m_windowId != VideoManager::kInvalidWindowId);
		
		Locator::videoManager->setWindowFullScreen(m_windowId, fullScreen);
	}
	
}

//! Close the window
void WindowWidget::close(){
	CAP_THROW_ASSERT(Locator::videoManager != nullptr, "VideoManager is null");
	Locator::videoManager->closeWindow(m_windowId);
	m_windowId = VideoManager::kInvalidWindowId;

	// call signal to notify listeners
	m_windowClosedSignal(this);
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
	CAP_THROW_NULL(pLayout, "Layout is null");
	
	m_pLayout = pLayout;
	m_pLayout->setParent(this);
	m_pLayout->setWindowId(m_windowId);
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
	m_width  = width;
	m_height = height;

	// if the  window has already been created, resize it
	if(m_windowId != VideoManager::kInvalidWindowId){
		CAP_THROW_ASSERT(Locator::videoManager != nullptr, "VideoManager is null");
		Locator::videoManager->setWindowSize(m_windowId, m_x, m_y);
	}
}

//! Updates the size of a window.)
/** 

This doesn't actually resize the window like setSize().

 \param width - The width of the Window.
 \param - The height of fthe Window
*/
void WindowWidget::updateSize(int width, int height){
	m_width  = width;
	m_height = height;

	if(m_pLayout)
		m_pLayout->setSize(width, height);
}

//! \copydoc WindowWidget::update
void WindowWidget::update(double ms){
	if(m_pLayout)
		m_pLayout->update(ms);
}

//! @copydoc Widget::render()
void WindowWidget::render(){
	if(m_shown){
		CAP_THROW_NULL(Locator::videoManager, "VideoManager is null");
		CAP_THROW_ASSERT(m_windowId != VideoManager::kInvalidWindowId, "WindowId is invalid");

		if(m_pLayout)
			m_pLayout->render();
	}
}

//! @copydoc Widget::handleMouseMotionEvent()
void WindowWidget::handleMouseMotionEvent(SDL_MouseMotionEvent event) {
	if(event.windowID == m_windowId)
		m_pLayout->handleMouseMotionEvent(event);
}

//! @copydoc Widget::handleMouseButtonEvent()
void WindowWidget::handleMouseButtonEvent(SDL_MouseButtonEvent event) {
	if(event.windowID == m_windowId)	
		m_pLayout->handleMouseButtonEvent(event);
}

//! @copydoc Widget::handleMouseWheelEvent()
void WindowWidget::handleMouseWheelEvent(SDL_MouseWheelEvent event) {
	if(event.windowID == m_windowId)	
		m_pLayout->handleMouseWheelEvent(event);
}

//! @copydoc Widget::handleKeyboardEvent()
void WindowWidget::handleKeyboardEvent(SDL_KeyboardEvent event) {
	if(event.windowID == m_windowId)
		m_pLayout->handleKeyboardEvent(event);
}

//! \copydoc Widget::handleTextInputEvent()
void WindowWidget::handleTextInputEvent(SDL_TextInputEvent event){
	if(event.windowID == m_windowId)
		m_pLayout->handleTextInputEvent(event);
}

//! @copydoc Widget::handleWindowEvent()
void WindowWidget::handleWindowEvent(SDL_WindowEvent event) {
	if(event.windowID == m_windowId){

		if(event.event == SDL_WINDOWEVENT_CLOSE){
			this->close();
			m_shown = false;
		}

		else{
			CAP_THROW_NULL(Locator::videoManager, "VideoManager is null");
			
			int width = Locator::videoManager->getWindowWidth(m_windowId);
			int height = Locator::videoManager->getWindowHeight(m_windowId);

			this->updateSize(width, height);
		}
	}
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

	m_handleTextInputConnection =
		eventSubscriber.m_textInputEventSignal.connect(std::bind(&WindowWidget::handleTextInputEvent, this, std::placeholders::_1));
}


//! Registers a slot with the window closed signal.
/** 
 \param slot
   \li The slot to register.
 \return 
   \li the signal connection objet
*/
boost::signals2::scoped_connection WindowWidget::registerWindowClosedSignal(std::function<void(WindowWidget*)> slot){
	return m_windowClosedSignal.connect(slot);
}

// \copydoc Widget::getChildren
std::vector<std::shared_ptr<Widget>> WindowWidget::getChildren() {
	return { m_pLayout };
}

//! Widget::getPosition
SDL_Rect WindowWidget::getPosition() const {
	return { 0, 0, m_width, m_height };
}

}} // namespace CapEngine::UI

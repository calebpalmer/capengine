#include "widgetstate.h"

#include "game_management.h"

namespace CapEngine { namespace UI {


//! Constructor
/** 
 \param onLoadFunctor - Code to call when loading the GameState
 \param onDestroyFunctor - Function to call when destroying the GameState
*/
WidgetState::WidgetState(std::function<bool(WidgetState& widgetState)> onLoadFunctor,
						std::function<bool(WidgetState& widgetState)> onDestroyFunctor) :
	m_onLoadFunctor(onLoadFunctor), m_onDestroyFunctor(onDestroyFunctor) {}


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
}

//! @copydoc GameState::update()
void WidgetState::update(double ms){
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
	auto pWindowWidget = CapEngine::createWindow(name, width, height, resizable);
	CAP_THROW_NULL(pWindowWidget, "Error creating window.  Window is null");
	m_pWindows.push_back(pWindowWidget);

	return pWindowWidget;
}

}} // namespace CapEngine::UI

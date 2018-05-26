#ifndef CAPENGINE_WIDGETSTATE_H
#define CAPENGINE_WIDGETSTATE_H

#include "gamestate.h"

#include "windowwidget.h"
#include "control.h"

#include <functional>
#include <boost/signals2/signal.hpp>
#include <boost/optional.hpp>

namespace CapEngine { namespace UI {

class WidgetState : public GameState {
 public:
	static std::shared_ptr<WidgetState> create(std::function<bool(WidgetState& widgetState)> onLoadFunctor,
																			std::function<bool(WidgetState& widgetState)> onDestroyFunctor);
	
	void render() override;
	void update(double ms) override;
	bool onLoad() override;
	bool onDestroy() override;

	std::shared_ptr<WindowWidget> createWindow(const std::string &name, int width, int height, bool resizable=true);
	void addControl(std::shared_ptr<UI::Control> pControl);
	boost::optional<std::shared_ptr<UI::Control>> popControl();
	boost::optional<std::shared_ptr<UI::Control>> peekControl();

	boost::signals2::scoped_connection connectPostRenderSignal(std::function<void(WidgetState&)> slot);

	virtual void handleMouseMotionEvent(SDL_MouseMotionEvent event);
	virtual void handleMouseButtonEvent(SDL_MouseButtonEvent event);
	virtual void handleMouseWheelEvent(SDL_MouseWheelEvent event);
	virtual void handleKeyboardEvent(SDL_KeyboardEvent event);
	virtual void handleWindowEvent(SDL_WindowEvent event);

 public:
	static constexpr char const* kControlStackLocatorId = "WindowWidgetControlStack";
	static constexpr char const* kWidgetStateLocatorId = "WidgetState";

protected:
	WidgetState();
	WidgetState(std::function<bool(WidgetState& widgetState)> onLoadFunctor,
							std::function<bool(WidgetState& widgetState)> onDestroyFunctor);

	//! The list of windows
	std::vector<std::shared_ptr<UI::WindowWidget>> m_pWindows;
	//! callback function that is called when widget state is loaded
	std::function<bool(WidgetState& widgetState)> m_onLoadFunctor;
	//! callback function that is called when widget state is destroyed
	std::function<bool(WidgetState& widgetState)> m_onDestroyFunctor;
	//! ui controls managed by this state
	std::shared_ptr<std::vector<std::shared_ptr<UI::Control>>> m_pUiControls;
	//! uiControl to add to stack
	boost::optional<std::shared_ptr<UI::Control>> m_pQueuedUiControl;
	
	//! signal that is called after state is rendered
	boost::signals2::signal<void(WidgetState&)> m_postRenderSignal;

	//signal connections
	//! connection for keyboard events
	boost::signals2::scoped_connection m_keyboardEventConnection;
	//! connection for mouse button events
	boost::signals2::scoped_connection m_mouseButtonEventConnection;
	//! connection for mouse motion events
	boost::signals2::scoped_connection m_mouseMotionEventConnection;
	//! connection for mouse wheel events
	boost::signals2::scoped_connection m_mouseWheelEventConnection;
	//! connection for window events
	boost::signals2::scoped_connection m_windowEventConnection;
	
};

}} // namespace CapEngine

#endif // CAPENGINE_WIDGETSTATE_H

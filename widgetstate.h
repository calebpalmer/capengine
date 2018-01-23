#ifndef CAPENGINE_WIDGETSTATE_H
#define CAPENGINE_WIDGETSTATE_H

#include "gamestate.h"

#include "windowwidget.h"

#include <functional>

namespace CapEngine { namespace UI {

class WidgetState : public GameState {
 public:
	WidgetState(std::function<bool(WidgetState& widgetState)> onLoadFunctor,
							std::function<bool(WidgetState& widgetState)> onDestroyFunctor);

	void render() override;
	void update(double ms) override;
	bool onLoad() override;
	bool onDestroy() override;

	std::shared_ptr<WindowWidget> createWindow(const std::string &name, int width, int height, bool resizable=true);

private:
	std::vector<std::shared_ptr<UI::WindowWidget>> m_pWindows;
	std::function<bool(WidgetState& widgetState)> m_onLoadFunctor;
	std::function<bool(WidgetState& widgetState)> m_onDestroyFunctor;
};

}} // namespace CapEngine

#endif // CAPENGINE_WIDGETSTATE_H

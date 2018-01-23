#include "locator.h"
#include "game_management.h"
#include "gamestate.h"
#include "windowwidget.h"
#include "VideoManager.h"
#include "widgetstate.h"

#include <memory>

namespace {

bool onLoad(CapEngine::UI::WidgetState &widgetState){
	std::string name = "Widget Test";
	int width = 640;
	int height = 480;
	auto pWindow = widgetState.createWindow(name,  width, height);
	pWindow->show();
	
	return true;
}

bool onDestroy(CapEngine::UI::WidgetState &widgetState){
	return true;
}

} // anonymous namespace

int main(){
	using namespace CapEngine;

	WindowParams params;
	CapEngine::init(params, true);

	std::unique_ptr<WidgetState> pWidgetTestState(new UI::WidgetState(onLoad, onDestroy));
	CapEngine::startLoop(std::move(pWidgetTestState));
}

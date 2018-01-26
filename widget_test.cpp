#include "locator.h"
#include "game_management.h"
#include "gamestate.h"
#include "VideoManager.h"
#include "CapEngineException.h"
#include "windowwidget.h"
#include "widgetstate.h"
#include "absolutelayout.h"
#include "label.h"


#include <memory>
#include <iostream>

namespace {

using namespace CapEngine;

bool onLoad(CapEngine::UI::WidgetState &widgetState){
	std::string name = "Widget Test";
	int width = 640;
	int height = 480;
	auto pWindow = widgetState.createWindow(name,  width, height);
	pWindow->show();

	auto pAbsLayout = std::make_shared<UI::AbsoluteLayout>(pWindow.get());
	pWindow->setLayout(pAbsLayout);

	auto pLabel = std::make_shared<UI::Label>(pAbsLayout.get(), "test", "res/fonts/tahoma.ttf");
	pAbsLayout->addWidget(pLabel, 10, 10, 200, 100);
	
	return true;
}

bool onDestroy(CapEngine::UI::WidgetState &widgetState){
	return true;
}

} // anonymous namespace

int main(){
	using namespace CapEngine;

	try{
		WindowParams params;
		CapEngine::init(params, true);

		std::unique_ptr<UI::WidgetState> pWidgetTestState(new UI::WidgetState(onLoad, onDestroy));
		CapEngine::startLoop(std::move(pWidgetTestState));
	}
	catch(const boost::exception &e){
		std::cerr << boost::diagnostic_information(e);		
	}
}

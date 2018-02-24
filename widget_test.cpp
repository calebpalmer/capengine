#include "locator.h"
#include "game_management.h"
#include "gamestate.h"
#include "VideoManager.h"
#include "CapEngineException.h"
#include "windowwidget.h"
#include "widgetstate.h"
#include "absolutelayout.h"
#include "gridlayout.h"
#include "linearlayout.h"
#include "label.h"

#include <memory>
#include <iostream>

namespace {

const std::string kFontPath = "res/fonts/tahoma.ttf";

using namespace CapEngine;

bool onLoad(CapEngine::UI::WidgetState &widgetState){
	std::string name = "Widget Test";
	int width = 640;
	int height = 480;
	auto pWindow = widgetState.createWindow(name,  width, height);
	pWindow->show();

	//auto pAbsLayout = UI::AbsoluteLayout::create();
	std::shared_ptr<UI::GridLayout> pGridLayout = UI::GridLayout::create(2, 2);
	pWindow->setLayout(pGridLayout);

	auto pLabel = UI::Label::create("test", kFontPath);
	//pAbsLayout->addWidget(pLabel, 10, 10, 200, 100);
	pGridLayout->addWidget(pLabel, 1, 0);

	std::shared_ptr<UI::LinearLayout> pLinearLayout =
		UI::LinearLayout::create(CapEngine::UI::LinearLayout::Orientation::Vertical);
	std::shared_ptr<UI::Label> pLabelLeft = UI::Label::create("Left", kFontPath);
	std::shared_ptr<UI::Label> pLabelRight = UI::Label::create("Right", kFontPath);
	pLinearLayout->addWidget(pLabelLeft);
	pLinearLayout->addWidget(pLabelRight);

	pGridLayout->addWidget(pLinearLayout, 0, 1);
	
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
#include "editor.h"

#include "CapEngineException.h"
#include "locator.h"
#include "logger.h"
#include "game_management.h"
#include "editorconstants.h"
#include "windowwidget.h"
#include "widgetstate.h"
#include "absolutelayout.h"
#include "gridlayout.h"
#include "linearlayout.h"
#include "label.h"
#include "uibutton.h"
#include "orientable.h"

#include <memory>
#include <iostream>
#include <sstream>
#include <exception>
#include <functional>

using namespace CapEngine;

namespace {

const std::string kMainWindowName = "main";
const std::string kMapWindowName = "map";

}// anonymous namespace

EditorArgs EditorArgs::parseArgs(int argc, char* argv[]){
  std::string tilesetPath = "";
  std::string mapPath = "";

  const int numPosArgs = 2;
  if(argc < numPosArgs + 1){
    std::ostringstream errorMsg;
    errorMsg << "Usage: " << argv[0] << "[tilesetpath] [mappath]" ;
    throw CapEngineException(errorMsg.str());
  }

  tilesetPath = argv[1];
  mapPath = argv[2];

  return EditorArgs(tilesetPath, mapPath);
}

Editor::Editor(EditorArgs args)
  : m_tilesetPath(args.m_tileset), m_mapPath(args.m_map)
{
	m_pMap.reset(new Map2D(m_mapPath));
}

//! called by WidgetState when starting up.
/** 
 \return - true if successful, false otherwise
*/
bool Editor::onLoad(CapEngine::UI::WidgetState &widgetState){

	std::shared_ptr<UI::WindowWidget> pWindow = widgetState.createWindow("Editor", 640, 480);
	pWindow->show();

	std::shared_ptr<UI::LinearLayout> pLinearLayout = UI::LinearLayout::create(UI::LinearLayout::Orientation::Vertical);
	std::shared_ptr<UI::MapPanel> pMapPanel = UI::MapPanel::create(this->m_pMap);
	pLinearLayout->addWidget(pMapPanel);
	
	pWindow->setLayout(pLinearLayout);
	
	return true;
}


//! Called by WidgetState when being destroyed.
/** 
 \return - true if successful, false otherwise.
*/
bool Editor::onDestroy(CapEngine::UI::WidgetState &widgetState){
	return true;
}

int main(int argc, char* argv[]){
  try{
    WindowParams windowParams;
		CapEngine::init(windowParams, true);
		
    auto args = EditorArgs::parseArgs(argc, argv);
		Editor editor(args);

		std::unique_ptr<UI::WidgetState>
			pWidgetState(new UI::WidgetState(std::bind(&Editor::onLoad, &editor, std::placeholders::_1), std::bind(&Editor::onDestroy, &editor, std::placeholders::_1)));
		CapEngine::startLoop(std::move(pWidgetState));		
  }
  catch(const boost::exception& e){
    std::cerr << boost::diagnostic_information(e) << std::endl;
    return -1;
  }
  catch(...){
    std::cerr << "Unknown exception caught." << std::endl;
		return -1;
  }
}

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
#include "control.h"

#include <memory>
#include <iostream>
#include <sstream>
#include <exception>
#include <functional>
#include <boost/polymorphic_pointer_cast.hpp>

namespace CapEngine { namespace UI {

namespace {

const std::string kMainWindowName = "main";
const std::string kMapWindowName = "map";

}// anonymous namespace

EditorArgs EditorArgs::parseArgs(int argc, char* argv[]){
  std::string tilesetPath = "";
  std::string mapPath = "";

  const int numPosArgs = 1;
  if(argc < numPosArgs + 1){
    std::ostringstream errorMsg;
    errorMsg << "Usage: " << argv[0] << "[tilesetpath] [mappath]" ;
    throw CapEngineException(errorMsg.str());
  }

  mapPath = argv[1];

  return EditorArgs(mapPath);
}

Editor::Editor(const EditorArgs &args)
  : WidgetState(), m_mapPath(args.m_map)
{
	m_pMap.reset(new Map2D(m_mapPath));
	m_pTileset = m_pMap->getTileSet();
}


//! creates an Editor WidgetState
/** 
 \param args
   The parsed command line arguments.
 \return 
   The pointer to a new Editor object.
*/
std::shared_ptr<Editor> Editor::create(const EditorArgs &args){
	std::shared_ptr<Editor> pEditor(new Editor(args));
	Locator::insert(kWidgetStateLocatorId, boost::polymorphic_pointer_cast<WidgetState>(pEditor));
	return pEditor;
}

//! called by WidgetState when starting up.
/** 
 \return - true if successful, false otherwise
*/
bool Editor::onLoad(){

	std::shared_ptr<UI::WindowWidget> pWindow = this->createWindow("Editor", 1280, 800);
	pWindow->show();

	std::shared_ptr<UI::GridLayout> pLayout = UI::GridLayout::create(1, 2);
	pWindow->setLayout(pLayout);
	
	m_pTileSetPanel = UI::TileSetPanel::create(this->m_pTileset);
	m_pMapPanel = UI::MapPanel::create(this->m_pMap);
	pLayout->addWidget(m_pTileSetPanel, 0, 0);
	pLayout->addWidget(m_pMapPanel, 0, 1);
	
	return true;
}


//! Called by WidgetState when being destroyed.
/** 
 \return - true if successful, false otherwise.
*/
bool Editor::onDestroy(){
	return true;
}

}} // CapEngine::UI

int main(int argc, char* argv[]){
  try{
		CapEngine::WindowParams windowParams;
		CapEngine::init(windowParams, true);
		
    auto args = CapEngine::UI::EditorArgs::parseArgs(argc, argv);

		// std::shared_ptr<CapEngine::UI::WidgetState> pWidgetState =
		// 	CapEngine::UI::WidgetState::create(std::bind(&CapEngine::UI::Editor::onLoad, &editor, std::placeholders::_1), std::bind(&CapEngine::UI::Editor::onDestroy, &editor, std::placeholders::_1));
		std::shared_ptr<CapEngine::UI::Editor> pEditor = CapEngine::UI::Editor::create(args);
		CapEngine::startLoop(pEditor);
  }
  catch(const boost::exception& e){
    std::cerr << boost::diagnostic_information(e) << std::endl;
    return -1;
  }
	catch(const std::exception &e){
		std::cerr << boost::diagnostic_information(e) << std::endl;
		return -1;
	}
	return 0;
}


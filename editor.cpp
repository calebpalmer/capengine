#include "editor.h"

#include <memory>
#include <iostream>
#include <sstream>

using namespace CapEngine;

EditorArgs EditorArgs::parseArgs(int argc, char* argv[]){
  std::string tilesetPath = "";
  std::string mapPath = "";

  const int numPosArgs = 2;
  if(argc < numPosArgs + 1){
    std::ostringstream errorMsg;
    errorMsg << "Usage: " << argv[0] << " [mappath] [tilesetpath]";
    throw CapEngineException(errorMsg.str());
  }

  tilesetPath = argv[1];
  mapPath = argv[2];

  return EditorArgs(tilesetPath, mapPath);
}

Editor::Editor(EditorArgs args)
  : m_tilesetPath(args.m_tileset), m_mapPath(args.m_map){  }

void Editor::receiveEvent(SDL_Event event, Time* time) {

}

void Editor::render(){
  m_pMapPanel->render();
}

void Editor::update(double ms) {}

bool Editor::onLoad() {
  return true;
}

bool Editor::onDestroy() {
  return true;
}

int main(int argc, char* argv[]){
  auto args = EditorArgs::parseArgs(argc, argv);
  
  WindowParams windowParams;
  windowParams.windowName = "CapEngine Editor";
  windowParams.width=800;
  windowParams.height=600;
  windowParams.pDepth = 32;
  windowParams.fullScreen = false;
  windowParams.opengl = false;
  windowParams.resizable = true;

  auto mainWindowID = CapEngine::init(windowParams);

  std::unique_ptr<GameState> pEditorState(new Editor(args));
  CapEngine::startLoop(std::move(pEditorState));

  return 1;
}

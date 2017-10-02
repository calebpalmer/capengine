#include "editor.h"

#include "CapEngineException.h"
#include "locator.h"
#include "logger.h"
#include "game_management.h"
#include "editorconstants.h"

#include <memory>
#include <iostream>
#include <sstream>
#include <exception>
#include <functional>

using namespace CapEngine;

namespace {

const std::string kMainWindowName = "main";
const std::string kMapWindowName = "map";

void getScreenSize(int displayNum, int& out_width, int& out_height){
  // get screen measurements
  SDL_DisplayMode displayMode;
  if(SDL_GetCurrentDisplayMode(displayNum, &displayMode) != 0){
    std::ostringstream msg;
    msg << "Unable to get display resolution " << SDL_GetError();
    BOOST_THROW_EXCEPTION(CapEngineException(msg.str()));
  }

  out_width = displayMode.w;
  out_height = displayMode.h;
}

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
  m_windowNameToIdMap.emplace(kMainWindowName, args.m_windowId);
}

void Editor::receiveEvent(SDL_Event event, Time* time) {
  switch(event.type){
  case SDL_KEYUP:
    {
      SDL_KeyboardEvent *keyboardEvent = reinterpret_cast<SDL_KeyboardEvent*>(&event);
      m_keyboardEventSignal(*keyboardEvent);
      break;
    }
  case SDL_KEYDOWN:
    {
      SDL_KeyboardEvent *keyboardEvent = reinterpret_cast<SDL_KeyboardEvent*>(&event);
      m_keyboardEventSignal(*keyboardEvent);
      break;
    }
  case SDL_MOUSEMOTION:
    {
      SDL_MouseMotionEvent *mouseMotionEvent = reinterpret_cast<SDL_MouseMotionEvent*>(&event);
      m_mouseMotionEventSignal(*mouseMotionEvent);
      break;
    }
  case SDL_MOUSEBUTTONUP:
    {
      SDL_MouseButtonEvent *mouseButtonEvent = reinterpret_cast<SDL_MouseButtonEvent*>(&event);
      m_mouseButtonEventSignal(*mouseButtonEvent);
      break;
    }
  case SDL_MOUSEBUTTONDOWN:
    {
      SDL_MouseButtonEvent *mouseButtonEvent = reinterpret_cast<SDL_MouseButtonEvent*>(&event);
      m_mouseButtonEventSignal(*mouseButtonEvent);
      break;
    }
  case SDL_MOUSEWHEEL:
    {
      SDL_MouseWheelEvent *mouseWheelEvent = reinterpret_cast<SDL_MouseWheelEvent*>(&event);
      m_mouseWheelEventSignal(*mouseWheelEvent);
      break;
    }
  case SDL_WINDOWEVENT:
    {
      SDL_WindowEvent *windowEvent = reinterpret_cast<SDL_WindowEvent*>(&event);
      m_windowEventSignal(*windowEvent);
      break;
    }
  }
}

void Editor::render(){
  auto&& windowId = m_windowNameToIdMap[kMainWindowName];
  auto&& videoManager = Locator::videoManager;
  videoManager->clearScreen(windowId);
  Rect rect = {0, 0, videoManager->getWindowWidth(windowId),
	       videoManager->getWindowHeight(windowId) };
  videoManager->drawRect(windowId, rect, kBackgroundColour);
  
  
  if(m_pMapPanel)
    m_pMapPanel->render();
}

void Editor::update(double ms) {

}

bool Editor::onLoad() {
  // load tileset
  m_tileset.reset(new TileSet(m_tilesetPath));
  m_map.reset(new Map2D(m_mapPath));

  // create window for map and then create the panel
  int screenNum = 0;
  int screenWidth = 0;
  int screenHeight = 0;
  getScreenSize(screenNum, screenWidth, screenHeight);
  
  WindowParams windowParams = {
    "MapPanel",
    screenWidth * 2 / 3,
    screenHeight * 2 / 3,
    32, // bpp,
    false, // fullscreen,
    false, // opengl
    false, // resizable
    "MapPanel"
  };

  auto mapWindowId = Locator::videoManager->createNewWindow(windowParams);
  m_windowNameToIdMap.emplace(kMainWindowName, mapWindowId);
  
  int posX = 0;
  int posY = 0;
  posX = screenWidth + (screenWidth * 1 / 3);
  posY = screenHeight + (screenHeight * 1 / 3);
  m_pMapPanel.reset(new MapPanel(mapWindowId,true, posX, posY,
				 screenWidth * 2 / 3, screenHeight * 2 / 3,
				 m_map));
  this->connectAllSignals(*m_pMapPanel);

  // subscribe to events
  int subscriptionMask = mouseEvent &
    keyboardEvent & systemEvent & windowEvent;
  IEventSubscriber::subscribe(Locator::eventDispatcher, subscriptionMask);
  
  return true;
}

bool Editor::onDestroy() {
  return true;
}

void Editor::connectAllSignals(Widget& widget){
  //using namespace std::placeholders;
  // need to fully qualify placeholders because it conflicts with boost2
  m_mouseMotionEventSignal.connect(std::bind(&Widget::handleMouseMotionEvent, &widget, std::placeholders::_1));
  m_mouseButtonEventSignal.connect(std::bind(&Widget::handleMouseButtonEvent, &widget, std::placeholders::_1));
  m_mouseWheelEventSignal.connect(std::bind(&Widget::handleMouseWheelEvent, &widget, std::placeholders::_1));
  m_keyboardEventSignal.connect(std::bind(&Widget::handleKeyboardEvent, &widget, std::placeholders::_1));
  m_windowEventSignal.connect(std::bind(&Widget::handleWindowEvent, &widget, std::placeholders::_1));
}

int main(int argc, char* argv[]){
  try{
    auto args = EditorArgs::parseArgs(argc, argv);

    WindowParams windowParams;
    windowParams.windowName = "CapEngine Editor";
    windowParams.width= 50;
    windowParams.height=50;
    windowParams.pDepth = 32;
    windowParams.fullScreen = false;
    windowParams.opengl = false;
    windowParams.resizable = true;

    auto mainWindowID = CapEngine::init(windowParams);
    args.m_windowId = mainWindowID;

    SDL_Window *window = SDL_GetWindowFromID(mainWindowID);
    CAP_THROW_ASSERT(window != nullptr, SDL_GetError());

    int screenWidth = 0;
    int screenHeight = 0;
    int screenNum = 0;
    getScreenSize(screenNum, screenWidth, screenHeight);

    SDL_SetWindowSize(window, screenWidth * 2 / 3, 50);
    SDL_SetWindowPosition(window, screenWidth * 1 / 3 / 2, 10);
    

    std::unique_ptr<GameState> pEditorState(new Editor(args));
    CapEngine::startLoop(std::move(pEditorState));

    return 1;
  }
  catch(const boost::exception& e){
    std::cerr << boost::diagnostic_information(e) << std::endl;
    return -1;
  }
  // TODO: this won't compile for some reason
  // catch(const std::exception& e){
  //   std::cerr << e.what() << std::end;
  //   return -1;
  // }
  catch(...){
    std::cerr << "Unknown exception caught." << std::endl;
  }
}

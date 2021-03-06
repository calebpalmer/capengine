#include "editor.h"

#include "CapEngineException.h"
#include "absolutelayout.h"
#include "control.h"
#include "editorconstants.h"
#include "game_management.h"
#include "gridlayout.h"
#include "keyboard.h"
#include "label.h"
#include "linearlayout.h"
#include "locator.h"
#include "logger.h"
#include "map2d.h"
#include "orientable.h"
#include "property.h"
#include "uibutton.h"
#include "uicommon.h"
#include "uiutils.h"
#include "widgetstate.h"
#include "windowwidget.h"

#include <boost/polymorphic_pointer_cast.hpp>
#include <exception>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>

namespace CapEngine
{
namespace UI
{

namespace
{

const std::string kMainWindowName = "main";
const std::string kMapWindowName = "map";

} // namespace

EditorArgs EditorArgs::parseArgs(int argc, char *argv[])
{
  std::string tilesetPath = "";
  std::string mapPath = "";

  const int numPosArgs = 1;
  if (argc < numPosArgs + 1) {
    std::ostringstream errorMsg;
    errorMsg << "Usage: " << argv[0] << "[tilesetpath] [mappath]";
    throw CapEngineException(errorMsg.str());
  }

  mapPath = argv[1];

  return EditorArgs(mapPath);
}

Editor::Editor(const EditorArgs &args) : WidgetState(), m_mapPath(args.m_map)
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
std::shared_ptr<Editor> Editor::create(const EditorArgs &args)
{
  std::shared_ptr<Editor> pEditor(new Editor(args));
  Locator::insert(kWidgetStateLocatorId,
                  boost::polymorphic_pointer_cast<WidgetState>(pEditor));
  return pEditor;
}

//! called by WidgetState when starting up.
/**
 \return - true if successful, false otherwise
*/
bool Editor::onLoad()
{

  bool resizable = true;
  std::shared_ptr<WindowWidget> pWindow =
      this->createWindow("Editor", 1280, 800, resizable);
  assert(pWindow != nullptr);
  pWindow->setMaximized(true);
  pWindow->show();

  std::shared_ptr<GridLayout> pLayout =
      UI::GridLayout::create(1, 3, boost::none, std::vector<int>{20, 60, 20});
  assert(pLayout != nullptr);
  pLayout->setBorder(BorderStyle::Solid, 2);
  pWindow->setLayout(pLayout);

  assert(m_pTileset != nullptr);
  m_pTileSetPanel = UI::TileSetPanel::create(this->m_pTileset);

  assert(m_pMap != nullptr);
  m_pMapPanel = UI::MapPanel::create(this->m_pMap);

  pLayout->addWidget(m_pTileSetPanel, 0, 0);
  pLayout->addWidget(m_pMapPanel, 0, 1);

  std::vector<Property> mapProperties = getMapProperties(m_pMap);
  std::shared_ptr<Widget> pMapPropertyGrid = createPropertyGrid(mapProperties);
  pLayout->addWidget(std::move(pMapPropertyGrid), 0, 2);

  return true;
}

//! Called by WidgetState when being destroyed.
/**
 \return - true if successful, false otherwise.
*/
bool Editor::onDestroy() { return true; }

//! \copydoc WidgetState::handleKeyboardEvent
void Editor::handleKeyboardEvent(SDL_KeyboardEvent event)
{
  WidgetState::handleKeyboardEvent(event);
  assert(Locator::keyboard != nullptr);
  bool lCtrlPressed = Locator::keyboard->key(Keyboard::CAP_LCTRL).state ==
                      Keyboard::CAP_PRESSED;

  if (event.type == SDL_KEYDOWN) {
    switch (event.keysym.sym) {
    case SDLK_s:
      doSave();
      break;
    case SDLK_z:
      if (lCtrlPressed)
        m_commandManager.undo();
      break;
    case SDLK_y:
      if (lCtrlPressed)
        m_commandManager.redo();
      break;
      {
        assert(Locator::keyboard != nullptr);
      }
      {
        assert(Locator::keyboard != nullptr);
      }
    }
  }
}

//! Save the map
void Editor::doSave()
{
  this->showOkCancelDialog("Save?", [this](bool ok) {
    if (ok) {
      assert(this->m_pMap != nullptr);
      this->m_pMap->save();
    }
  });
}

//! Gets the command manager
/**
 \return
   \li The CommandManager
*/
CommandManager &Editor::getCommandManager() { return m_commandManager; }

} // namespace UI
} // namespace CapEngine

int main(int argc, char *argv[])
{
  try {
    CapEngine::WindowParams windowParams;
    CapEngine::init(windowParams, true);

    auto args = CapEngine::UI::EditorArgs::parseArgs(argc, argv);

    std::shared_ptr<CapEngine::UI::Editor> pEditor =
        CapEngine::UI::Editor::create(args);
    CapEngine::startLoop(pEditor);
  } catch (const boost::exception &e) {
    std::cerr << boost::diagnostic_information(e) << std::endl;
    return -1;
  } catch (const std::exception &e) {
    std::cerr << boost::diagnostic_information(e) << std::endl;
    return -1;
  }
  return 0;
}

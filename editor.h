#ifndef EDITOR_H
#define EDITOR_H

#include <memory>

#include "map_panel.h"
#include "tilesetpanel.h"
#include "tileset.h"
#include "map2d.h"
#include "gamestate.h"
#include "widgetstate.h"
#include "commandmanager.h"

#include <map>
#include <boost/signals2.hpp>

namespace CapEngine { namespace UI {

  class EditorArgs {
  public:
  EditorArgs(std::string mapPath) :
    m_map(mapPath) {}

    static EditorArgs parseArgs(int argc, char* argv[]);

    std::string m_map;
    Uint32 m_windowId = -1;
  };

  class Editor : public WidgetState {
  public:
		static std::shared_ptr<Editor> create(const EditorArgs &args);
		
		bool onLoad() override;
		bool onDestroy() override;

		void handleKeyboardEvent(SDL_KeyboardEvent event) override;

		CommandManager& getCommandManager();
		
  private:
    Editor(const EditorArgs &args);

		void doSave();

		//! The path to the map configuration path
    std::string m_mapPath;
		//! The tileset
    std::shared_ptr<TileSet> m_pTileset;
		//! The map
		std::shared_ptr<Map2D> m_pMap;
		//! The map panel
    std::shared_ptr<UI::MapPanel> m_pMapPanel;
		//! The tileset panel
		std::shared_ptr<UI::TileSetPanel> m_pTileSetPanel;
		//! command manager for doing undo/redo
		CommandManager m_commandManager;
	};
}} // CapEngine::UI

#endif // EDITOR_H

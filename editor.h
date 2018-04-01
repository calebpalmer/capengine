#ifndef EDITOR_H
#define EDITOR_H

#include <memory>

#include "map_panel.h"
#include "tilesetpanel.h"
#include "tileset.h"
#include "map2d.h"
#include "gamestate.h"
#include "widgetstate.h"

#include <map>
#include <boost/signals2.hpp>

namespace CapEngine {

  class EditorArgs{
  public:
  EditorArgs(std::string tilesetPath, std::string mapPath) :
    m_tileset(tilesetPath), m_map(mapPath) {}

    static EditorArgs parseArgs(int argc, char* argv[]);

    std::string m_tileset;
    std::string m_map;
    Uint32 m_windowId = -1;
  };

  class Editor {
  public:
    Editor(EditorArgs args);
		bool onLoad(CapEngine::UI::WidgetState &widgetState);
		bool onDestroy(CapEngine::UI::WidgetState &widgetState);
  private:
    std::string m_tilesetPath;
    std::string m_mapPath;
    std::shared_ptr<TileSet> m_pTileset;
    std::shared_ptr<Map2D> m_pMap;
    std::shared_ptr<UI::MapPanel> m_pMapPanel;
		std::shared_ptr<UI::TileSetPanel> m_pTileSetPanel;
};
}

#endif // EDITOR_H

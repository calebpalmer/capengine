#ifndef EDITOR_H
#define EDITOR_H

#include <memory>

#include "map_panel.h"
#include "tileset.h"
#include "map2d.h"
#include "gamestate.h"

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

  class Editor : public IEventSubscriber, public GameState {
  public:
    Editor(EditorArgs args);
    virtual void receiveEvent(SDL_Event event, CapEngine::Time* time) override;
    virtual void render() override;
    virtual void update(double ms) override;
    virtual bool onLoad() override;
    virtual bool onDestroy() override;

  private:
    Editor(const Editor&);
    Editor& operator=(const Editor&);

    void connectAllSignals(Widget& widget);

    std::unique_ptr<MapPanel> m_pMapPanel;
    std::string m_tilesetPath;
    std::string m_mapPath;
    std::shared_ptr<TileSet> m_tileset;
    std::shared_ptr<Map2D> m_map;
    std::map<std::string, Uint32> m_windowNameToIdMap;

    // signals
    boost::signals2::signal<void(SDL_KeyboardEvent)> m_keyboardEventSignal;
    boost::signals2::signal<void(SDL_MouseMotionEvent)> m_mouseMotionEventSignal;
    boost::signals2::signal<void(SDL_MouseButtonEvent)> m_mouseButtonEventSignal;
    boost::signals2::signal<void(SDL_MouseWheelEvent)> m_mouseWheelEventSignal;
    boost::signals2::signal<void(SDL_WindowEvent)> m_windowEventSignal;
  };
}

#endif // EDITOR_H

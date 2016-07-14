#gifndef EDITOR_H
#define EDITOR_H

#include <memory>

#include "CapEngine.h"
#include "map_panel.h"

namespace CapEngine {

  class EditorArgs{
  public:
  EditorArgs(std::string tilesetPath, std::string mapPath) :
    m_tileset(tilesetPath), m_map(mapPath) {}

    static EditorArgs parseArgs(int argc, char* argv[]);

    std::string m_tileset;
    std::string m_map;
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

    std::unique_ptr<MapPanel> m_pMapPanel;
    std::string m_tilesetPath;
    std::string m_mapPath;

  };
}

#endif // EDITOR_H

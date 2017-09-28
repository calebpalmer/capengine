#ifndef MAP_PANEL_H
#define MAP_PANEL_H

#include <string>

#include <SDL2/SDL.h>

#include "IEventSubscriber.h"

namespace CapEngine{

  class MapPanel : public IEventSubscriber {
  public:
    MapPanel(Uint32 windowID, int x, int y, int width, int height, std::string mapPath);
    ~MapPanel() = default;
    void resize(int x, int y, int w, int h);
    void render();

    virtual void receiveEvent(SDL_Event event, CapEngine::Time* time) override;

  private:
    Uint32 m_windowID;
    std::string m_mapPath;
    int m_x;
    int m_y;
    int m_width;
    int m_height;

  };
}

#endif // MAP_PANEL_H

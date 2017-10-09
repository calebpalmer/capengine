#ifndef MAP_PANEL_H
#define MAP_PANEL_H

#include "IEventSubscriber.h"
#include "vector.h"
#include "map2d.h"
#include "widget.h"

#include <string>
#include <vector>
#include <SDL2/SDL.h>


namespace CapEngine{

  class MapPanel : public Widget {
  public:
    MapPanel(Uint32 windowID, bool ownsWindow, int x, int y, int width, int height, std::shared_ptr<Map2D> pMap);
    ~MapPanel() = default;
    void resize(int x, int y, int w, int h);
    void render();

    void handleMouseMotionEvent(SDL_MouseMotionEvent event) override;

  private:
    void drawTileOutline();
    
    Uint32 m_windowID = -1;
    std::shared_ptr<Map2D> m_pMap;
    bool m_ownsWindow = false;
    int m_x= 0;
    int m_y = 0;
    int m_width = 0;
    int m_height = 0;
    double m_scaleFactor = 1;
    CapEngine::Vector m_translationMatrix = {0.0, 0.0, 0.0};
    std::pair<int, int> m_hoveredTile = {-1, -1};
  };
}

#endif // MAP_PANEL_H

#ifndef MAP_PANEL_H
#define MAP_PANEL_H

#include "IEventSubscriber.h"
#include "vector.h"
#include "map2d.h"
#include "widget.h"
#include "matrix.h"

#include <string>
#include <vector>
#include <SDL2/SDL.h>

namespace CapEngine{

  enum DragState {
    DRAGSTATE_NONE,
    DRAGSTATE_SELECT,
    DRAGSTATE_PAN,
  };

  class MapPanel : public Widget {
  public:
    MapPanel(Uint32 windowID, bool ownsWindow, int x, int y, int width, int height, std::shared_ptr<Map2D> pMap);
    ~MapPanel() = default;
    void resize(int x, int y, int w, int h);
    void render();

    void handleMouseMotionEvent(SDL_MouseMotionEvent event) override;
    void handleMouseButtonEvent(SDL_MouseButtonEvent event) override;
    void handleKeyboardEvent(SDL_KeyboardEvent event) override;
    void handleMouseWheelEvent(SDL_MouseWheelEvent event) override;
    
  private:
    void drawHoveredTileOutline();
    void drawSelectedTileOutlines();
    void drawMouseDrag();
    std::pair<int, int> getHoveredTile(int x, int y) const;

    void handleLeftMouseButtonUp(SDL_MouseButtonEvent event);
    void handleLeftMouseButtonDown(SDL_MouseButtonEvent event);
    void handleMiddleMouseButtonUp(SDL_MouseButtonEvent event);
    void handleMiddleMouseButtonDown(SDL_MouseButtonEvent event);

    bool isInMap(int x, int y) const;
    
    Uint32 m_windowID = -1;
    std::shared_ptr<Map2D> m_pMap;
    bool m_ownsWindow = false;
    int m_x= 0;
    int m_y = 0;
    int m_width = 0;
    int m_height = 0;

    CapEngine::Matrix m_translationMatrix = Matrix::createIdentityMatrix();
    CapEngine::Matrix m_scaleMatrix = Matrix::createIdentityMatrix();
    
    std::pair<int, int> m_hoveredTile = {-1, -1};
    std::vector<std::pair<int, int>> m_selectedTiles;

    DragState m_dragState = DRAGSTATE_NONE;
    std::pair<int, int> m_dragStart = {-1, -1};
  };
}

#endif // MAP_PANEL_H

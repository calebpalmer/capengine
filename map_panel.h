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
		void drawTileOutlines(const std::vector<std::pair<int, int>> &tiles,
													CapEngine::Colour colour);
    void drawMouseDrag();
    std::pair<int, int> getHoveredTile(int x, int y) const;

    void handleLeftMouseButtonUp(SDL_MouseButtonEvent event);
    void handleLeftMouseButtonDown(SDL_MouseButtonEvent event);
    void handleMiddleMouseButtonUp(SDL_MouseButtonEvent event);
    void handleMiddleMouseButtonDown(SDL_MouseButtonEvent event);

    bool isInMap(int x, int y) const;


		//<! The window id to displauy on
    Uint32 m_windowID = -1;
		//<! the map that is being edited
    std::shared_ptr<Map2D> m_pMap;
		//<! flag indicating if this panel owns the whole window.
    bool m_ownsWindow = false;
		//<! TODO
    int m_x= 0;
		//<! TODO
    int m_y = 0;
		//<! TODO
    int m_width = 0;
		//<! TODO
    int m_height = 0;

		//<! Matrix used for the maps location in the window
    CapEngine::Matrix m_translationMatrix = Matrix::createIdentityMatrix();
		//<! Matrix usedd for the maps current scaling amount
    CapEngine::Matrix m_scaleMatrix = Matrix::createIdentityMatrix();

		//<! The tile the cursor is hovering over
    std::pair<int, int> m_hoveredTile = {-1, -1};
		//<! The currently selected tiles
    std::vector<std::pair<int, int>> m_selectedTiles;

		//<! The current state of dragging
    DragState m_dragState = DRAGSTATE_NONE;
		//<! the cursor location where dragging started
    std::pair<int, int> m_dragStart = {-1, -1};
		//<! The location of the cursor after the last motion event
		std::pair<int, int> m_lastMotionLocation = {0.0, 0.0};
  };
}

#endif // MAP_PANEL_H

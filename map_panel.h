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

namespace CapEngine { namespace UI {

  enum DragState {
    DRAGSTATE_NONE,
    DRAGSTATE_SELECT,
    DRAGSTATE_PAN,
  };

  class MapPanel : public UI::Widget {
  public:
		static std::shared_ptr<MapPanel> create(std::shared_ptr<Map2D> pMap);
    ~MapPanel() = default;

		virtual void setPosition(int x, int y) override;
		virtual void setSize(int width, int height) override;
		virtual void render() override;
		virtual void update(double ms) override;

    virtual void handleMouseMotionEvent(SDL_MouseMotionEvent event) override;
    virtual void handleMouseButtonEvent(SDL_MouseButtonEvent event) override;
    virtual void handleKeyboardEvent(SDL_KeyboardEvent event) override;
    virtual void handleMouseWheelEvent(SDL_MouseWheelEvent event) override;
    
  private:
    MapPanel(std::shared_ptr<Map2D> pMap);

		void doLocationInit();
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
		SDL_Rect getMapExtents() const;
		SDL_Rect getVisibleMapExtents() const;
		double getScaledTileSize() const;
		std::vector<std::tuple<int, int, bool, SDL_Rect>> getTileRects() const;
		std::vector<std::pair<int, int>> getTilesInRect(const SDL_Rect &rect) const;

		//! the map that is being edited
    std::shared_ptr<Map2D> m_pMap;
		//! flag indicated first location init
		bool m_locationInitialized = false;

		//! The panel starting x position
		int m_panelX = 0;
		//! The panel starting y position
		int m_panelY = 0;
		//! The panel width
		int m_panelWidth = 0;
		//! the panel height
		int m_panelHeight = 0;
		
		//! The x position of the map within the panel
    int m_x= 0;
		//! The y position of the map within the panel
    int m_y = 0;
		//! The width of the map
    int m_width = 0;
		//! The height of the map
    int m_height = 0;

		//! Matrix used for the maps location in the window
    CapEngine::Matrix m_translationMatrix = Matrix::createIdentityMatrix();
		//! Matrix usedd for the maps current scaling amount
    CapEngine::Matrix m_scaleMatrix = Matrix::createIdentityMatrix();

		//! The tile the cursor is hovering over
    std::pair<int, int> m_hoveredTile = {-1, -1};
		//! The currently selected tiles
    std::vector<std::pair<int, int>> m_selectedTiles;
		//! Tiles that should be outlined (drag operation?)
		std::vector<std::pair<int, int>> m_outlinedTiles;

		//! The current state of dragging
    DragState m_dragState = DRAGSTATE_NONE;
		//! the cursor location where dragging started
    std::pair<int, int> m_dragStart = {-1, -1};
  };
	
}}

#endif // MAP_PANEL_H

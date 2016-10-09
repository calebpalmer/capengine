#include "map_panel.h"

using namespace CapEngine;

MapPanel::MapPanel(Uint32 windowID, int x, int y, int width, int height, std::string mapPath)
  : m_windowID(windowID), m_x(x), m_y(y), m_width(width), m_height(height)
    , m_mapPath(mapPath) {}



/**
 resizes the panel 
 */
void MapPanel::resize(int x, int y, int w, int h){
  m_x = x;
  m_y = y;
  m_width = w;
  m_height = h;
}

/**
 * Render the map panel
 */
void MapPanel::render(){}


/**
 * handle SDL events 
 */
void MapPanel::receiveEvent(SDL_Event event, CapEngine::Time* time) {

}


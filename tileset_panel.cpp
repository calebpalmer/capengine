#include "tileset_panel.h"

using namespace CapEngine;

TilesetPanel::TilesetPanel(Uint32 windowID, bool ownsWindow, int x, int y, int w, int h, std::string tilesetFile)
  : m_windowID(windowID), m_tilesetPath(tilesetFile),
    m_ownsWindow(ownsWindow), m_x(x), m_y(y), m_w(w), m_h(h)
{
  

}

/**
   Resize the panel
 */
void TilesetPanel::resize(int x, int y, int w, int h){
  m_x = x;
  m_y = y;
  m_w = w;
  m_h = h;
}

/**
   render the panel
 */
void TilesetPanel::render(){
  
}


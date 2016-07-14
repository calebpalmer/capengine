#ifndef TILESET_PANEL_H
#define TILESET_PANEL_H

#include <string>
#include <SDL2/SDL.h>

namespace CapEngine{

  class TilesetPanel{
  public:

    TilesetPanel(Uint32 windowID, int x, int y, int w, int h, std::string tilesetFile);
    void resize(int x, int y, int w, int h);
    void render();

  private:
    Uint32 m_windowID;
    std::string m_tilesetPath;
    int m_x;
    int m_y;
    int m_w;
    int m_h;
  };
}

#endif // TILESET_PANEL_H

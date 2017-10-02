#ifndef TILESET_PANEL_H
#define TILESET_PANEL_H

#include <string>
#include <SDL2/SDL.h>

namespace CapEngine{

  class TilesetPanel{
  public:

    TilesetPanel(Uint32 windowID, bool ownsWindow,
		 int x, int y, int w, int h, std::string tilesetFile);
    void resize(int x, int y, int w, int h);
    void render();

  private:
    Uint32 m_windowID = -1;
    std::string m_tilesetPath;
    bool m_ownsWindow = true;
    int m_x = 0;
    int m_y = 0;
    int m_w = 0;
    int m_h = 0;
  };
}

#endif // TILESET_PANEL_H

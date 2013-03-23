#ifndef MAP2D_H
#define MAP2D_H

#include <memory>
#include <fstream>

#include "tileset.h"
#include "captypes.h"

namespace CapEngine{
  
  class Map2D{

    std::string configPath;
    std::string tileSetPath;
    std::unique_ptr<TileSet> tileSet;

    Map2D(const Map2D& map);
    Map2D& operator=(const Map2D& map);

    void readTiles(std::ifstream& stream);
    void drawSurface();

  public:
    struct TileTup{
      Tile* tile;
      unsigned int index;
    };

    std::vector<TileTup*> tiles;
    Surface* surface;
    unsigned int width;
    unsigned int height;

    Map2D(const std::string mapConfigPath, bool loadVideo = true);
    ~Map2D();
    std::string toString();
  };

}

#endif //MAP2D_H

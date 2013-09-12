#ifndef MAP2D_H
#define MAP2D_H

#include <memory>
#include <fstream>

#include "tileset.h"
#include "captypes.h"
#include "collision.h"
#include "VideoManager.h"

namespace CapEngine{
  
  class Map2D{

    std::string configPath;
    std::string tileSetPath;
    std::unique_ptr<TileSet> tileSet;
    Logger logger;
    VideoManager* videoManager;
    

    Map2D(const Map2D& map);
    Map2D& operator=(const Map2D& map);
    void readTiles(std::ifstream& stream);
    void drawSurface();
    std::unique_ptr<Rectangle> getTileMBR(int index);

  public:
    struct TileTup{
      Tile* tile;
      unsigned int index;
    };

    struct CollisionTup{
      Tile* tile;
      CollisionType collisionType;
    };

    std::vector<TileTup*> tiles;
    Surface* surface;
    unsigned int width;
    unsigned int height;

    Map2D(const std::string mapConfigPath, VideoManager* videoManagerIn = nullptr);
    ~Map2D();
    std::string toString();
    std::vector<CollisionTup> getCollisions(const Rectangle& mbr);
    
  };

}

#endif //MAP2D_H

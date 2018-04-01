#ifndef MAP2D_H
#define MAP2D_H

#include "tileset.h"
#include "captypes.h"
#include "collision.h"
#include "VideoManager.h"

#include <memory>
#include <fstream>

namespace CapEngine{
  
  class Map2D{

  public:

    enum TileLookupStatus{
      TileLookupStatus_Found,
      TileLookupStatus_NotFound,
      TileLookupStatus_NoTile
    };
      
    struct TileTup{
      Tile tile;
      int index;
      TileLookupStatus tileLookupStatus;
    };
    
    struct CollisionTup{
      Tile tile;
      CollisionType collisionType;
    };

    Map2D(const std::string mapConfigPath);
    ~Map2D();

    // deleted
    Map2D(const Map2D& map) = delete;
    Map2D& operator=(const Map2D& map) = delete;

    std::string toString();
    std::vector<CollisionTup> getCollisions(const Rectangle& mbr);
    Surface* getSurface();
    int getWidth() const;
    int getHeight() const;
    int getTileSize() const;

    void setWidth(int width);
    void setHeight(int height);
    void deleteTile(int x, int y);
    
		std::shared_ptr<TileSet> getTileSet();
		
  private:
    void readTiles(std::ifstream& stream);
    void drawSurface();
    std::unique_ptr<Rectangle> getTileMBR(int index);

    std::string configPath;
    std::string tileSetPath;
    std::shared_ptr<TileSet> tileSet;
    std::vector<std::vector<TileTup>> tiles;
    Surface* surface;
    unsigned int width;
    unsigned int height;
  };

}

#endif //MAP2D_H

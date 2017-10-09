#ifndef TILESET_H
#define TILESET_H

#include <string>
#include <vector>
#include <memory>

#include "captypes.h"
#include "VideoManager.h"

namespace CapEngine{

  enum TileType{
    TILE_NORMAL,
    TILE_SOLID
  };

  struct Tile {
    int xpos;
    int ypos;
    int width;
    int height;
    TileType type;
  };


  // TODO:  This class needs to be refactored to hide it's data members
  class TileSet {
  public:
    ~TileSet();
    TileSet(const std::string& configPath);

    bool tileExists(unsigned int index);
    Tile getTile(unsigned int index);
    unsigned int getTileSize() const;

    std::string configFilepath;
    std::string surfaceFilepath;
    unsigned int tileCount;
    unsigned int tileWidth;
    unsigned int tileHeight;
    std::vector<Tile> tiles;
    Surface* surface;
      
  private:
    TileSet(const TileSet&);
    TileSet& operator=(TileSet);
    Tile parseTile(const std::string& line);
    void validate();

    VideoManager* videoManager;
	
    };
}

#endif //TILESET

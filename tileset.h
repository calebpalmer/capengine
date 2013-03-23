#ifndef TILESET_H
#define TILESET_H

#include <string>
#include <vector>
#include <memory>

#include "captypes.h"
#include "VideoManager.h"

namespace CapEngine{
  class Tile {
  public:
    int xpos;
    int ypos;
    int width;
    int height;
  };


  class TileSet {
  public:
    std::string configFilepath;
    std::string surfaceFilepath;
    unsigned int tileCount;
    std::vector<Tile*> tiles;
    
  public:  // public functions
    Surface* surface;

    ~TileSet();
    TileSet(const std::string& configPath, bool loadVideo = false);
    bool tileExists(unsigned int index);
    Tile& getTile(unsigned int index);
      
  private:  // private functions
      Tile& parseTile(const std::string& line);
      void validate();
    };
}

#endif //TILESET

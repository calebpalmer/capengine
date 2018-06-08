#ifndef TILESET_H
#define TILESET_H

#include <string>
#include <vector>
#include <memory>

#include "captypes.h"
#include "VideoManager.h"

#include <memory>

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


	//! A class the represents a TileSet.
  class TileSet {
  public:
    TileSet(const std::string& configPath);

    bool tileExists(unsigned int index) const;
    Tile getTile(unsigned int index) const;
    unsigned int getTileSize() const;
		unsigned int getNumTiles() const;

		std::shared_ptr<SDL_Surface> getSurface();
		std::string getConfigFilePath() const;
		std::string getSurfaceFilePath() const;
		const std::vector<Tile>& getTiles() const;
		unsigned int getTileWidth() const;
		unsigned int getTileHeight() const;
      
  private:
    TileSet(const TileSet&);
    TileSet& operator=(TileSet);
    Tile parseTile(const std::string& line);
    void validate();

		//! The surface with the tiles
		std::shared_ptr<SDL_Surface> m_pSurface;
		//! The path to the configuration file
    std::string m_configFilePath;
		//! The path to the image with the tiles
    std::string m_surfaceFilePath;
		//! The tiles
    std::vector<Tile> m_tiles;
		//! The number off tiles
    unsigned int m_tileCount;
		//! The width of the tiles
    unsigned int m_tileWidth;
		//! The height of the tiles
    unsigned int m_tileHeight;
		
    };
}

#endif //TILESET

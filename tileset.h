#ifndef TILESET_H
#define TILESET_H

#include <string>
#include <vector>
#include <memory>

#include "captypes.h"
#include "VideoManager.h"

#include <memory>
#include <jsoncons/json.hpp>

namespace CapEngine{

  enum TileType{
    TILE_NORMAL,
    TILE_SOLID,
		TILE_UNKNOWN
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
		TileSet(const jsoncons::json &json);

		TileSet(const TileSet &other);
		TileSet(TileSet&& other) = default;

		TileSet& operator=(const TileSet &other);
		TileSet& operator=(TileSet &&other) = default;

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

		void save(const std::string &filepath);
		jsoncons::json json() const;
      
  private:
    Tile parseTile(const std::string& line);
    void validate();
		void loadSurface();
		void copy(const TileSet &tileset);

		//! The surface with the tiles
		std::shared_ptr<SDL_Surface> m_pSurface;
		//! The path to the configuration file
    std::string m_configFilePath;
		//! The path to the image with the tiles
    std::string m_surfaceFilePath;
		//! The tiles
    std::vector<Tile> m_tiles;
		//! The number off tiles
    unsigned int m_tileCount = 0;
		//! The width of the tiles
    unsigned int m_tileWidth = 0;
		//! The height of the tiles
    unsigned int m_tileHeight = 0;
		
    };
}

#endif //TILESET

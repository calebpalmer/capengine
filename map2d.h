#ifndef MAP2D_H
#define MAP2D_H

#include "tileset.h"
#include "captypes.h"
#include "collision.h"
#include "VideoManager.h"
#include "CapEngineException.h"
#include "property.h"

#include <memory>
#include <fstream>
#include <jsoncons/json.hpp>

namespace CapEngine{

  //! Exception class for when an invalid location index into the map.
  class MapIndexException : public CapEngineException {
  public:
    MapIndexException(int x, int y);
    const char* what();
  private:
    int m_x; //!< The x index.
    int m_y; //!< The y index.
  };

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

	public:
    Map2D(const std::string &mapConfigPath);
		Map2D(const Map2D& map);
    ~Map2D();

    // deleted

    Map2D& operator=(const Map2D& map) = delete;

    std::string toString();
    std::vector<CollisionTup> getCollisions(const Rectangle& mbr);
    Surface* getSurface();
    int getWidth() const;
    int getHeight() const;
    int getTileSize() const;
		int getTileIndex(int x, int y) const;
		
		bool isDirty() const;

    void setWidth(int width);
    void setHeight(int height);
    void deleteTile(int x, int y);
    void setTile(int x, int y, int tileSetIndex);

		std::shared_ptr<TileSet> getTileSet();
		std::string getTileSetPath() const;

		void save(const std::string &filepath="") const;
		jsoncons::json json() const;
		
  private:
		void load(jsoncons::json json);
    void drawSurface();
    std::unique_ptr<Rectangle> getTileMBR(int index);

    std::string configPath;
    std::string tileSetPath;
    std::shared_ptr<TileSet> tileSet;
    std::vector<std::vector<TileTup>> tiles;
    Surface* surface = nullptr;
    bool m_surfaceDirty = true;
    unsigned int width = 0;
    unsigned int height = 0;
		//! flag indicating if data has been changed since last save.
		mutable bool m_isDirty = false;
  };

std::vector<Property> getMapProperties(std::shared_ptr<Map2D> pMap);
}

#endif //MAP2D_H

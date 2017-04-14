#ifndef PLATFORMERMAP_H
#define PLATFORMERMAP_H

#include <vector>

#include "vector.h"

namespace CapEngine {

  class PlatformerMap {
  public:
    PlatformerMap(Uint32 windowID, int mapAssetID, int collisionMapAssetID);
    ~PlatformerMap();
    void update(double ms);
    void render();
    std::vector<Vector> getSpawnPoints() const;
    void addSpawnPoint(Vector spawnPoint);
    Surface* getCollisionMap();
    int getWidth();
    int getHeight();
    
  private:
    Uint32 m_windowID;
    int m_mapAssetID;
    int m_collisionMapAssetID;
    Surface* m_collisionMapSurface;
    std::vector<Vector> m_spawnPoints;
    int m_width;
    int m_height;
  };
}

#endif // PLATFORMERMAP_H

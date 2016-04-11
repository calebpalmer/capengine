#ifndef PLATFORMERMAP_H
#define PLATFORMERMAP_H

#include <vector>

#include "vector.h"

namespace CapEngine {

  class PlatformerMap {
  public:
    PlatformerMap(int mapAssetID, int collisionMapAssetID);
    ~PlatformerMap();
    void update(double ms);
    void render();
    std::vector<Vector> getSpawnPoints() const;
    void addSpawnPoint(Vector spawnPoint);
    Surface* getCollisionMap();
    
  private:
    int m_mapAssetID;
    int m_collisionMapAssetID;
    Surface* m_collisionMapSurface;
    std::vector<Vector> m_spawnPoints;
  };
}

#endif // PLATFORMERMAP_H

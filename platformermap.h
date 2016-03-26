#ifndef PLATFORMERMAP_H
#define PLATFORMERMAP_H

#include <vector>

#include "vector.h"

namespace CapEngine {

  class PlatformerMap {
  public:
    PlatformerMap(int mapAssetID, int collisionMapAssetID);
    void update(double ms);
    void render();
    std::vector<Vector> getSpawnPoints() const;
    void addSpawnPoint(Vector spawnPoint);
    
  private:
    int m_mapAssetID;
    int m_collisionMapAssetID;
    std::vector<Vector> m_spawnPoints;
  };
}

#endif // PLATFORMERMAP_H

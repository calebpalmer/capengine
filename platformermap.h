#ifndef PLATFORMERMAP_H
#define PLATFORMERMAP_H

namespace CapEngine {

  class PlatformerMap {
  public:
    PlatformerMap(int mapAssetID, int collisionMapAssetID);
    void update(double ms);
    void render();
    
  private:
    int m_mapAssetID;
    int m_collisionMapAssetID;
  };
}

#endif // PLATFORMERMAP_H

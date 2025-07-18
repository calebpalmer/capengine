#ifndef PLATFORMERMAP_H
#define PLATFORMERMAP_H

#include <memory>
#include <vector>

#include "captypes.h"
#include "mapobjectdescriptor.h"
#include "vector.h"

namespace CapEngine {

class PlatformerMap {
   public:
    static std::unique_ptr<PlatformerMap> createPlatformerMapFromFile(std::string const& in_arenaConfigPath,
                                                                      int in_arenaId);

    PlatformerMap(int mapAssetID, int collisionMapAssetID, int in_finishLineX);
    ~PlatformerMap();

    void update(double ms);
    void render(Uint32 in_windowId);

    std::vector<Vector> getSpawnPoints() const;
    void addSpawnPoint(Vector spawnPoint);
    const std::vector<MapObjectDescriptor>& getObjectDescriptors();
    Surface* getCollisionMap();
    int getWidth();
    int getHeight();

   private:
    int m_mapAssetID = -1;
    int m_collisionMapAssetID = -1;
    Surface* m_collisionMapSurface;
    std::vector<Vector> m_spawnPoints;
    std::vector<CapEngine::MapObjectDescriptor> m_objectDescriptors;
    int m_finishLineX = -1;
    int m_width = -1;
    int m_height = -1;
};
}  // namespace CapEngine

#endif // PLATFORMERMAP_H

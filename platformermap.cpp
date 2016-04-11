#include "platformermap.h"

#include "locator.h"
#include "asset_manager.h"
#include "vector.h"
#include "VideoManager.h"

using namespace CapEngine;
using namespace std;

PlatformerMap::PlatformerMap(int mapAssetID, int collisionMapAssetID)
  : m_mapAssetID(mapAssetID), m_collisionMapAssetID(collisionMapAssetID),
    m_collisionMapSurface(nullptr) { }

PlatformerMap::~PlatformerMap(){
  if(m_collisionMapSurface != nullptr){
    Locator::videoManager->closeSurface(m_collisionMapSurface);
  }
}

void PlatformerMap::update(double ms) {}

void PlatformerMap::render(){
  Locator::assetManager->draw(m_mapAssetID, Vector(0.0, 0.0));
}

vector<Vector> PlatformerMap::getSpawnPoints() const{
  return m_spawnPoints;
}

void PlatformerMap::addSpawnPoint(Vector spawnPoint){
  m_spawnPoints.push_back(spawnPoint);
}

Surface* PlatformerMap::getCollisionMap(){
  if(m_collisionMapSurface == nullptr){
    SoftwareImage softwareImage = Locator::assetManager->getSoftwareImage(m_collisionMapAssetID);
    m_collisionMapSurface = softwareImage.surface;
  }
  return m_collisionMapSurface;
}

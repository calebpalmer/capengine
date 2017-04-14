#include "platformermap.h"

#include "locator.h"
#include "asset_manager.h"
#include "vector.h"
#include "VideoManager.h"

using namespace CapEngine;
using namespace std;

PlatformerMap::PlatformerMap(Uint32 windowID, int mapAssetID, int collisionMapAssetID)
  : m_windowID(windowID), m_mapAssetID(mapAssetID)
  , m_collisionMapAssetID(collisionMapAssetID), m_collisionMapSurface(nullptr),
    m_width(-1), m_height(-1) {}

PlatformerMap::~PlatformerMap(){
  if(m_collisionMapSurface != nullptr){
    Locator::videoManager->closeSurface(m_collisionMapSurface);
  }
}

void PlatformerMap::update(double ms) {}

void PlatformerMap::render(){
  Locator::assetManager->draw(m_windowID, m_mapAssetID, Vector(0.0, 0.0));
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

int PlatformerMap::getWidth(){
  if(m_width == -1){
    m_width = Locator::assetManager->getImageWidth(m_mapAssetID);
  }
  return m_width;
}

int PlatformerMap::getHeight(){
  if(m_height == -1){
    m_height = Locator::assetManager->getImageHeight(m_mapAssetID);
  }
  return m_height;
}

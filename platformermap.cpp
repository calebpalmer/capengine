#include "platformermap.h"

#include "locator.h"
#include "asset_manager.h"
#include "vector.h"

using namespace CapEngine;

PlatformerMap::PlatformerMap(int mapAssetID, int collisionMapAssetID)
  : m_mapAssetID(mapAssetID), m_collisionMapAssetID(collisionMapAssetID) { }

void PlatformerMap::update(double ms) {}

void PlatformerMap::render(){
  Locator::assetManager->draw(m_mapAssetID, Vector(0.0, 0.0));
}

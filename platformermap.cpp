#include "platformermap.h"

#include "locator.h"
#include "asset_manager.h"
#include "vector.h"
#include "VideoManager.h"
#include "xml_parser.h"

#include <sstream>
#include <boost/lexical_cast.hpp>

using namespace CapEngine;
using namespace std;

PlatformerMap::PlatformerMap(int mapAssetID, int collisionMapAssetID, int in_finishLineX)
  : m_mapAssetID(mapAssetID), m_finishLineX(in_finishLineX)
  , m_collisionMapAssetID(collisionMapAssetID), m_collisionMapSurface(nullptr) {}

PlatformerMap::~PlatformerMap(){
  if(m_collisionMapSurface != nullptr){
    Locator::videoManager->closeSurface(m_collisionMapSurface);
  }
}

void PlatformerMap::update(double ms) {}

void PlatformerMap::render(Uint32 in_windowId){
  Locator::assetManager->draw(in_windowId, m_mapAssetID, Vector(0.0, 0.0));
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

std::unique_ptr<PlatformerMap> PlatformerMap::createPlatformerMapFromFile(
            std::string const& in_arenaConfigPath,
            int in_arenaId)
{
  std::unique_ptr<PlatformerMap> pPlatformerMap;
  // open config files
  XmlParser xmlParser(in_arenaConfigPath);

  // find arena
  stringstream xpathExpression;
  xpathExpression <<  "/arenas/arena[@id=" << in_arenaId << "]";
  auto nodes = xmlParser.getNodes(xpathExpression.str());
  if(nodes.size() < 1){
    stringstream errorMsg;
    errorMsg <<  "Arena with ID" << in_arenaId << " not found in " << in_arenaConfigPath;
    throw CapEngineException(errorMsg.str());
  }

  int mapAssetID = -1;
  int collisionMapAssetID = -1;
  vector<Vector> arenaSpawnPoints = {};
  int xFinishLocation = -1;

  auto children = xmlParser.getNodeChildren(nodes[0]);
  for (auto& i : children){

    // texture
    if(xmlParser.nodeNameCompare(i, "texture")){
      string temp = xmlParser.getStringValue(i);
      //mapAssetID = std::stoi(temp);
      mapAssetID = boost::lexical_cast<int>(temp);
    }
    // collision texture
    else if(xmlParser.nodeNameCompare(i, "collisions")){
      string temp = xmlParser.getStringValue(i);
      //collisionMapAssetID = std::stoi(temp);
      collisionMapAssetID = boost::lexical_cast<int>(temp);
    }
    // finish line
    else if(xmlParser.nodeNameCompare(i, "finish_location")){
      string temp = xmlParser.getStringValue(i);
      collisionMapAssetID = std::stoi(temp);
      xFinishLocation = boost::lexical_cast<int>(temp);
    }
    // spawn points
    else if(xmlParser.nodeNameCompare(i, "spawn_points")){
      auto spawnPoints = xmlParser.getNodeChildren(i);
      for(auto& spawnPoint : spawnPoints){
        if(xmlParser.nodeNameCompare(spawnPoint, "spawn_point")){
          try{
            int x = atoi((xmlParser.getAttribute(spawnPoint, "x")).c_str());
            int y = atoi((xmlParser.getAttribute(spawnPoint, "y")).c_str());
            arenaSpawnPoints.push_back(Vector(x, y));
          }
          catch(exception& e){
            Locator::logger->log(e.what(), Logger::CWARNING, __FILE__, __LINE__);
          }
        }
      }
    }
  }

  if(mapAssetID == -1 || collisionMapAssetID == -1){
    stringstream errorMsg;
    errorMsg << "Map Asset ID or Collision Map Asset ID not found in " << in_arenaConfigPath
         << " for area ID " << in_arenaId;
    throw CapEngineException(errorMsg.str());
  }


  //std::unique_ptr<PlatformerMap> platformerMap(new PlatformerMap(mapAssetID, collisionMapAssetID));
  pPlatformerMap.reset((new PlatformerMap(mapAssetID, collisionMapAssetID, xFinishLocation)));
  for(auto& spawnPoint : arenaSpawnPoints){
    pPlatformerMap->addSpawnPoint(spawnPoint);
  }

  return std::move(pPlatformerMap);
}



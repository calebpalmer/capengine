#include "platformermap.h"

#include "locator.h"
#include "asset_manager.h"
#include "vector.h"
#include "VideoManager.h"
#include "xml_parser.h"
#include "mapobjectdescriptor.h"

#include <sstream>
#include <boost/lexical_cast.hpp>

using namespace std;

namespace
{

//! parses spawnable objects
/**
* \param parser the xml parser to use to parse.
* \param xmlNode The node to parse objects from.  should have a list of <object> children.
* \return vector of CapEngine::MaoObjectDescriptors that were parsed.
*/
std::vector<CapEngine::MapObjectDescriptor> parseSpawnableObjects(CapEngine::XmlParser& parser,
																																	CapEngine::XmlNode& xmlNode)
{
	std::vector<CapEngine::MapObjectDescriptor> mapObjectDescriptors;
	
	std::vector<CapEngine::XmlNode> children = parser.getNodeChildren(xmlNode);
	for (auto && child : children)
	{
		if(parser.nodeNameCompare(child, "object"))
		{
			try
			{
				std::string objectClass = parser.getAttribute(child, "class");
				std::string objectType = parser.getAttribute(child, "type");
				int x =
					boost::lexical_cast<int>(parser.getAttribute(child, "x"));
				int y =
					boost::lexical_cast<int>(parser.getAttribute(child, "y"));

				CapEngine::MapObjectDescriptor mapObjectDescriptor(objectClass, objectType,
																													 CapEngine::Vector(x, y, 0, 1.0));
				mapObjectDescriptors.push_back(mapObjectDescriptor);
			}
			catch(const boost::bad_lexical_cast &e)
			{
				CAP_THROW_ASSERT(CapEngine::Locator::logger != nullptr, "Logger is null");

				std::stringstream msg;
				msg << "Error parsing spawnable object" << std::endl << e.what();
				CapEngine::Locator::logger->log(msg.str(), CapEngine::Logger::CWARNING);
			}
		}
	}

	return mapObjectDescriptors;
}

} // anonymous namespace

namespace CapEngine
{

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

//! get MapObjectDescriptors
/**
* \return A collection of Objects that the map spawns
*/
const std::vector<MapObjectDescriptor>& PlatformerMap::getObjectDescriptors()
{
	return m_objectDescriptors;
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
	std::vector<MapObjectDescriptor> objectDescriptors;

  auto children = xmlParser.getNodeChildren(nodes[0]);
  for (auto && i : children){

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
		// spawnable objects
		else if(xmlParser.nodeNameCompare(i, "objects"))
		{
			objectDescriptors = parseSpawnableObjects(xmlParser, i);
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
	pPlatformerMap->m_objectDescriptors = objectDescriptors;
  for(auto& spawnPoint : arenaSpawnPoints){
    pPlatformerMap->addSpawnPoint(spawnPoint);
  }

  return pPlatformerMap;
}

} // namespace CapEngine

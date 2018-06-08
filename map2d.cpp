#include "map2d.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "CapEngineException.h"
#include "filesystem.h"
#include "logger.h"
#include "locator.h"

#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

using namespace std;

namespace CapEngine{

namespace {

const char kWidthParameterName[] = "width";
const char kHeightParameterName[] = "height";
const char kTilesetParamaterName[] = "tileset";
const char kTileArrayParameterName[] = "tiles";
const char kIndexParameterName[] = "index";

//! return a TileTup for the given index into the tileset.
/** 
 \param tileset 
   \li The tileset object.
 \param index
   \li The index into the tileset
 \return
   \li the tiletup
*/
Map2D::TileTup lookupTile(const TileSet& tileset, int index){
	Map2D::TileTup tileTup;

	if(!tileset.tileExists(index)){
		tileTup.index = -1;
		tileTup.tileLookupStatus = Map2D::TileLookupStatus_NotFound;
	}
	else{
		tileTup.index = index;
		tileTup.tileLookupStatus = Map2D::TileLookupStatus_Found;
		tileTup.tile = tileset.getTile(index);
	}

	return tileTup;
}

} // namespace

//! Constructor.
/**
  \param x
    \li The x index into the map.
  \param y
    \li The y index into the map.
*/
MapIndexException::MapIndexException(int x, int y) 
  : CapEngineException(""), m_x(x), m_y(y)
{
}

//! Returns the exception message.
/**
  \return 
    \li The exception message.
*/
const char* MapIndexException::what(){
  return (boost::format("Invalid map index: %1%, %2%") % m_x % m_y).str().c_str();
}

Map2D::~Map2D(){
  CAP_THROW_ASSERT(Locator::videoManager != nullptr, "Video Manager is null");
  Locator::videoManager->closeSurface(surface);
}

//! Construct a map from json
/** 
 \param json
   \li The json with the map data
*/
void Map2D::load(jsoncons::json json){
	// this must be called from main constructor so do some asserts
	assert(!(this->configPath.empty()));

	// get header information
	if(!json.has_key(kWidthParameterName))
		BOOST_THROW_EXCEPTION(CapEngineException(std::string("Json missing property: ") + kWidthParameterName));
	this->width = json[kWidthParameterName].as<unsigned int>();

	if(!json.has_key(kHeightParameterName))
		BOOST_THROW_EXCEPTION(CapEngineException(std::string("Json missing property: ") + kHeightParameterName));
	this->height = json[kHeightParameterName].as<unsigned int>();

	if(!json.has_key(kTilesetParamaterName))
		BOOST_THROW_EXCEPTION(CapEngineException(std::string("Json missing property: ") + kTilesetParamaterName));
	this->tileSetPath = json[kTilesetParamaterName].as<std::string>();

	namespace fs = boost::filesystem;
	fs::path tileSetPath = fs::path(stripPath(configPath)) / fs::path(this->tileSetPath);
	if(!fs::exists(tileSetPath)){
		BOOST_THROW_EXCEPTION(CapEngineException((boost::format("Tileset path does not exist: %1%") % tileSetPath.string()).str()));
	}
	
	tileSet.reset(new TileSet(tileSetPath.string()));

	// make sure tileset path is not empty
	if(this->tileSetPath.empty())
		BOOST_THROW_EXCEPTION(CapEngineException((boost::format("The property %1% cannot be empty.") % kTilesetParamaterName).str()));
	
	if(!json.has_key(kTileArrayParameterName))
		BOOST_THROW_EXCEPTION(CapEngineException(std::string("Json missing property: ") + kTileArrayParameterName));

	// Get tiles
	jsoncons::json tileArray = json[kTileArrayParameterName];
	if(!tileArray.is_array())
		BOOST_THROW_EXCEPTION(CapEngineException("Invalid Tile Array"));

	int i = 0;
	for(auto && row : tileArray.array_range()){

		if(!row.is_array())
			BOOST_THROW_EXCEPTION(CapEngineException("Invalid row in Tile Array"));

		std::vector<TileTup> rowOfTiles;
		for(auto && tile : row.array_range()){
			if(!tile.has_key(kIndexParameterName))
				BOOST_THROW_EXCEPTION(CapEngineException((boost::format("Array value missing %1% property") % kIndexParameterName).str()));
				
			const int index = tile[kIndexParameterName].as<int>();
			
			assert(tileSet != nullptr);
			TileTup tileTup = lookupTile(*tileSet, index);
			rowOfTiles.push_back(std::move(tileTup));
		}

		tiles.push_back(std::move(rowOfTiles));
	}
	
}

Map2D::Map2D(const string mapConfigPath) : tileSet(nullptr) {
	// test that configPath exists and throw exception if it doesn't
  if(!fileExists(mapConfigPath)){
    BOOST_THROW_EXCEPTION(CapEngineException(mapConfigPath + " is not a valid path"));
  }

	configPath = mapConfigPath;

	// read config file
  ifstream configStream(mapConfigPath);
  if(!configStream.good()){
    throw CapEngineException("unable to open file: " + mapConfigPath);
  }

	jsoncons::json json;
	configStream >> json;
	this->load(json);
}

void Map2D::drawSurface(){
  if(Locator::videoManager->initialized == false){
    BOOST_THROW_EXCEPTION(CapEngineException("VideoManager not initialized"));
  }
  SDL_Surface* newSurface = Locator::videoManager->createSurface(width, height);

	std::shared_ptr<SDL_Surface> pTileSurface = tileSet->getSurface();
	assert(pTileSurface != nullptr);
	
  int rowNum = 0;
  for(auto && row : tiles){

    int columnNum = 0;
    for(auto && column : row){
      int destX = columnNum * tileSet->getTileSize();
      int destY = rowNum * tileSet->getTileSize();;
      Locator::videoManager->blitSurface(pTileSurface.get(), column.tile.xpos, column.tile.ypos, column.tile.width, column.tile.height, newSurface, destX, destY);
      columnNum++;
    }
    rowNum++;
  }

#ifdef DEBUG
  boost::filesystem::path path(this->configPath);
  boost::filesystem::path dir = path.parent_path();
  std::ostringstream filename;
  filename << path.stem() << ".bmp";
  boost::filesystem::path filePath = dir /= boost::filesystem::path(filename.str());
    
  Locator::videoManager->saveSurface(newSurface, filePath.string());
  std::ostringstream msg;
  msg << "Saved map surface as " << filePath;
  Locator::logger->log(msg.str(), Logger::CDEBUG);
#endif

  if(surface == nullptr)
    SDL_FreeSurface(surface);

  surface = newSurface;
  Locator::logger->log("Drew consolidated map texture", Logger::CDEBUG, __FILE__, __LINE__);
}

string Map2D::toString()
{
  unsigned int xRes = 0;
  ostringstream output;
  output << "width=" << width << endl
	 << "height="<< height << endl
	 << "tileset=" << tileSetPath << endl  // TODO Fix this, wrong path
	 << "tiles=" << endl;

  // TODO reimplement this
  // vector<TileTup>::iterator iter;
  // for(iter = tiles.begin(); iter != tiles.end(); iter++){
  //   if(xRes >= width){
  //     xRes = 0;
  //     output << endl;
  //   }
  //   output << iter->index << ',';
  //   xRes += iter->tile.width;  
  // }
  return output.str();
}

unique_ptr<Rectangle>  Map2D::getTileMBR(int index){
	assert(tileSet != nullptr);
	auto tileWidth = tileSet->getTileWidth();
	auto tileHeight = tileSet->getTileHeight();
	
  int tilesWide = width / tileWidth;
  int xpos = tileWidth * (index % tilesWide);
  int ypos = tileHeight * (index / tilesWide);
  
  return unique_ptr<Rectangle>(new Rectangle(xpos, ypos, tileWidth, tileHeight));
}

vector<Map2D::CollisionTup> Map2D::getCollisions(const Rectangle& mbr){
  // do brute force search for collisions of all map MBRs
  vector<CollisionTup> collisions;

  // TODO reimplement this
  // for(size_t i = 0; i < tiles.size();  i++){
  //   unique_ptr<Rectangle> tileMBR = getTileMBR(i);
  //   CollisionType collisionType = detectMBRCollision(mbr, *tileMBR);
  //   if(collisionType != COLLISION_NONE){
  //     CollisionTup collision;
  //     collision.collisionType = collisionType;
  //     collision.tile = tiles[i].tile;
  //     collisions.push_back(collision);
  //   }
  // }
  
  return collisions;
}

Surface* Map2D::getSurface(){

  if(m_surfaceDirty){
    drawSurface();
		m_surfaceDirty = false;
	}

  return surface;
}

int Map2D::getWidth() const{
  return width;
}

int Map2D::getHeight() const{
  return height;
}

void Map2D::setWidth(int newWidth){
  width = newWidth;
}

void Map2D::setHeight(int newHeight){
  height = newHeight;
}

int Map2D::getTileSize() const{
  CAP_THROW_ASSERT(tileSet.get() != nullptr, "TileSet is null");
  // TODO assuming tilesize is square.  Fix TileSet to be only square tiles.
  return tileSet->getTileWidth();
}

void Map2D::deleteTile(int x, int y){
  int tileSize = tileSet->getTileSize();
  if(x > boost::numeric_cast<int>(width) / tileSize
     || y > boost::numeric_cast<int>(height) / tileSize
     || x < 0 || y < 0)
  {
    BOOST_THROW_EXCEPTION(CapEngineException("Invalid Tile Index"));
  }

  TileTup tiletup;
  memset(&tiletup, 0, sizeof(TileTup));
  tiletup.tileLookupStatus = TileLookupStatus_NoTile;

  tiles[x][y] = tiletup;
}



//! Gets the TileSet used by this Map.
/** 
 \return The TileSet.
*/
std::shared_ptr<TileSet> Map2D::getTileSet(){
	return tileSet;
}

//! Set a tile at the given index.
/**
  \param x
    \li The x location of the tile to set.
  \param y
    \li The y location of the tile to set.
*/
void Map2D::setTile(int x, int y, int tileSetIndex){
  assert(tileSet != nullptr);
  int widthInTiles = this->width / tileSet->getTileWidth();
  int heightInTiles = this->height / tileSet->getTileHeight();

  if(x > widthInTiles || y > heightInTiles ||
    x < 0 || y < 0)
  {
    throw MapIndexException(x, y);
  }

  Tile tile = tileSet->getTile(tileSetIndex);

  TileTup tiletup;
  tiletup.tileLookupStatus =TileLookupStatus_Found;
  tiletup.index = tileSetIndex;
  tiletup.tile = tile;

  assert(static_cast<size_t>(y) < tiles.size());
  assert(static_cast<size_t>(x) < tiles[y].size());
  tiles[y][x] = tiletup;
	m_surfaceDirty =  true;
}

//! Saves the map to the given file.
/** 
 \param filepath
   \li the filepath to write to.
*/
void Map2D::save(const std::string &filepath) const{
	const std::string path = filepath == "" ? configPath : filepath;
	std::ofstream f(path);

	// header information
	f << this->json();
}


//! return json representationof the map 
/** 
 \return 
   \li The json representation of the map
*/
jsoncons::json Map2D::json() const{
	jsoncons::json json;

	json.insert_or_assign(kWidthParameterName, this->getWidth());
	json.insert_or_assign(kHeightParameterName, this->getHeight());
	json.insert_or_assign(kTilesetParamaterName, tileSetPath);

	// the tiles array
	jsoncons::json::array rows;
	for(auto && row : tiles){
		jsoncons::json::array cols;
		for(auto && tileTup : row){
			jsoncons::json tile;
			tile.insert_or_assign(kIndexParameterName, tileTup.index);
			cols.emplace_back(tile);
		}
		rows.emplace_back(cols);
	}

	json.insert_or_assign(kTileArrayParameterName, rows);

	return json;
}

} // namespace CapEngine

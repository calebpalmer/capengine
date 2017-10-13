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

using namespace std;

namespace CapEngine{

Map2D::~Map2D(){
  CAP_THROW_ASSERT(Locator::videoManager != nullptr, "Video Manager is null");
  Locator::videoManager->closeSurface(surface);
}

Map2D::Map2D(const string mapConfigPath) : tileSet(nullptr) {
  // test that configPath exists and throw exception if it doesn't
  if(!fileExists(mapConfigPath)){
    BOOST_THROW_EXCEPTION(CapEngineException(mapConfigPath + " is not a valid path"));
  }
  this->configPath = mapConfigPath;
  
  // read config file
  ifstream configStream(configPath);
  if(!configStream.good()){
    throw CapEngineException("unable to open file: " + configPath);
  }
  string line;
  while(configStream.good()){
    getline(configStream, line);
    if(line == ""){
      //TODO I think this should be continue
      break;
    }
    string::size_type position;
    position = line.find("=");

    if(position == string::npos || position == 0){
      Locator::logger->log("Invalid Line", Logger::CWARNING, __FILE__, __LINE__);
      break;
    }

    string parameter;
    parameter = line.substr(0, position);
    if(parameter == "width"){
      string value = line.substr(position + 1, line.size() - position);
      istringstream valueStream(value);
      valueStream >> width;
    }
    else if(parameter == "height"){
      string value = line.substr(position + 1, line.size() - position);
      istringstream valueStream(value);
      valueStream >> height;
    }
    else if(parameter == "tileset"){
      string value = line.substr(position + 1, line.size() - position);
      string mapPath = stripPath(configPath);
      string prevPath = getCurrentDir();
      setCurrentDir(mapPath);
      //      string tilesetPath = stripPath(configPath) + "/" + value;
      this->tileSetPath = value;
      tileSet.reset(new TileSet(this->tileSetPath));
      setCurrentDir(prevPath);
    }
    else if(parameter == "tiles"){
      readTiles(configStream);
    }
    else{
      // log warning
      Locator::logger->log("Invalid parameter: " + parameter, Logger::CWARNING, __FILE__, __LINE__);
    }
      
  }
  configStream.close();

  if(Locator::videoManager != nullptr){
    // load and draw texture
    if(Locator::videoManager->initialized == false){
      throw CapEngineException("VideoManager not initialized");
    }
    drawSurface();
  }
  ostringstream logString;
  logString << "loaded map from " << mapConfigPath << endl
	    << " using tileset " << tileSet->configFilepath << endl
	    << " with " << tiles.size() << " tiles loaded " << endl;
  Locator::logger->log(logString.str(), Logger::CDEBUG, __FILE__, __LINE__);
}

void Map2D::readTiles(ifstream& stream){
  int row = 0;
  int numRowsInMap = this->height / this->tileSet->getTileSize();
  
  while(stream.good() && row < numRowsInMap){

    std::vector<TileTup> rowOfTiles;
    string line;
    getline(stream, line);
    
    if(line == ""){
      row++;
      continue;
    }

    int numTilesInRow = this->width / this->tileSet->getTileSize();
    int column = 0;

    string::size_type position;
    string::size_type oldPosition = 0;
    position = line.find(',');
    
    while(position != string::npos && column < numTilesInRow){

      std::string value = line.substr(oldPosition, position - oldPosition);
      boost::trim_right(value);

      try{
	int tileIndex = boost::lexical_cast<int>(value);

	if(!tileSet->tileExists(tileIndex)){
	  Locator::logger->log("Unable to load map.  Tile does not exist: " + tileIndex, Logger::CWARNING);

	  TileTup tileTup;
	  memset(&tileTup, 0, sizeof(tileTup));
	  tileTup.index = -1;
	  tileTup.tileLookupStatus = TileLookupStatus_NotFound;
	  rowOfTiles.push_back(tileTup);
	  continue;
	}

	TileTup tileTup;
	tileTup.tile = tileSet->getTile(tileIndex);
	tileTup.index = tileIndex;
	tileTup.tileLookupStatus = TileLookupStatus_Found;
	rowOfTiles.push_back(tileTup);

      }

      catch(boost::bad_lexical_cast& e){
	TileTup tileTup;
	memset(&tileTup, 0, sizeof(tileTup));
	tileTup.index = -1;
	tileTup.tileLookupStatus = TileLookupStatus_NoTile;
	
	if(value != ""){
	  Locator::logger->log("Unknowne tile" + value, Logger::CWARNING);
	  tileTup.tileLookupStatus = TileLookupStatus_NotFound;
	}

	rowOfTiles.push_back(tileTup);
      }

      oldPosition = position + 1;
      position = line.find(',', oldPosition);
      column++;
    }

    // fill in unspecified tiles
    for(int i = rowOfTiles.size(); i < numTilesInRow; i++){
      TileTup tileTup;
      memset(&tileTup, 0, sizeof(TileTup));
      tileTup.tileLookupStatus = TileLookupStatus_NoTile;
    }
    
    tiles.push_back(rowOfTiles);
    row++;
  }
}

void Map2D::drawSurface(){
  unsigned int xRes, yRes = 0;
  if(Locator::videoManager->initialized == false){
    BOOST_THROW_EXCEPTION(CapEngineException("VideoManager not initialized"));
  }
  surface = Locator::videoManager->createSurface(width, height);

  int rowNum = 0;
  for(auto && row : tiles){

    int columnNum = 0;
    for(auto && column : row){
      int destX = columnNum * tileSet->getTileSize();
      int destY = rowNum * tileSet->getTileSize();;
      Locator::videoManager->blitSurface((tileSet->surface), column.tile.xpos, column.tile.ypos, column.tile.width, column.tile.height, surface, destX, destY);
      columnNum++;
    }
    rowNum++;
  }

  // vector<TileTup>::iterator iter;
  // for(iter = tiles.begin(); iter != tiles.end(); iter++){
  //   if(xRes >= width){
  //     xRes = 0;
  //     yRes += iter->tile.height;
  //   }
  //   Locator::videoManager->blitSurface((tileSet->surface), iter->tile.xpos, iter->tile.ypos, iter->tile.width, iter->tile.height, surface, xRes, yRes);
  //   xRes += iter->tile.width;
  // }

#ifdef DEBUG
  boost::filesystem::path path(this->configPath);
  boost::filesystem::path dir = path.parent_path();
  std::ostringstream filename;
  filename << path.stem() << ".bmp";
  boost::filesystem::path filePath = dir /= boost::filesystem::path(filename.str());
    
  Locator::videoManager->saveSurface(surface, filePath.string());
  std::ostringstream msg;
  msg << "Saved map surface as " << filePath;
  Locator::logger->log(msg.str(), Logger::CDEBUG);
#endif

  
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
  int tilesWide = width / tileSet->tileWidth;
  int xpos = tileSet->tileWidth * (index % tilesWide);
  int ypos = tileSet->tileHeight * (index / tilesWide);
  
  return unique_ptr<Rectangle>(new Rectangle(xpos, ypos, tileSet->tileWidth, tileSet->tileHeight));
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
  return tileSet->tileWidth;;
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


} // namespace CapEngine

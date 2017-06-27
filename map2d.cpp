#include "map2d.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "CapEngineException.h"
#include "filesystem.h"
#include "logger.h"
#include "locator.h"

using namespace CapEngine;
using namespace std;

Map2D::~Map2D(){
  vector<TileTup*>::iterator iter;
  for(iter = tiles.begin(); iter != tiles.end(); iter++){
    delete *iter;
  }
}

Map2D::Map2D(const string mapConfigPath) : tileSet(nullptr) {
  
  // test that configPath exists and throw exception if it doesn't
  if(!fileExists(mapConfigPath)){
    throw CapEngineException(mapConfigPath + " is not a valid path");
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
    drawTexture();
  }
  ostringstream logString;
  logString << "loaded map from " << mapConfigPath << endl
	    << " using tileset " << tileSet->configFilepath << endl
	    << " with " << tiles.size() << " tiles loaded " << endl;
  Locator::logger->log(logString.str(), Logger::CDEBUG, __FILE__, __LINE__);
}

void Map2D::readTiles(ifstream& stream){
  while(stream.good()){
    string line;
    getline(stream, line);
    
    if(line == ""){
      return;
    }

    string::size_type position;
    string::size_type oldPosition = 0;
    position = line.find(',');
    while(position != string::npos){
      istringstream valueStream(line.substr(oldPosition, position - oldPosition));
      unsigned int tileIndex;
      valueStream >> tileIndex;
      if(!tileSet->tileExists(tileIndex)){
			  throw CapEngineException("Unable to load map.  Tile does not exist: " + tileIndex);
      }
      unique_ptr<TileTup> sp_tileTup(new TileTup);
      sp_tileTup->tile = &(tileSet->getTile(tileIndex));
      sp_tileTup->index = tileIndex;
      tiles.push_back(sp_tileTup.release());

      oldPosition = position + 1;
      position = line.find(',', oldPosition);
    }
  }
}

void Map2D::drawTexture(){
  unsigned int xRes, yRes = 0;
  if(Locator::videoManager->initialized == false){
    throw CapEngineException("VideoManager not initialized");
  }
  Surface* surface = Locator::videoManager->createSurface(width, height);

  vector<TileTup*>::iterator iter;
  for(iter = tiles.begin(); iter != tiles.end(); iter++){
    if(xRes >= width){
      xRes = 0;
      yRes += (*iter)->tile->height;
    }
    Locator::videoManager->blitSurface((tileSet->surface), (*iter)->tile->xpos, (*iter)->tile->ypos, (*iter)->tile->width, (*iter)->tile->height, surface, xRes, yRes);
    xRes += (*iter)->tile->width;
  }
  texture = Locator::videoManager->createTextureFromSurface(surface);
  Locator::logger->log("Drew consolidate map texture", Logger::CDEBUG, __FILE__, __LINE__);
}

string Map2D::toString(){
	unsigned int xRes = 0;
	ostringstream output;
  output << "width=" << width << endl
	 << "height="<< height << endl
	 << "tileset=" << tileSetPath << endl  // TODO Fix this, wrong path
	 << "tiles=" << endl;
  
  vector<TileTup*>::iterator iter;
  for(iter = tiles.begin(); iter != tiles.end(); iter++){
    if(xRes >= width){
      xRes = 0;
      output << endl;
    }
    output << ((*iter)->index) << ',';
    xRes += (*iter)->tile->width;  
  }
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
  
  for(size_t i = 0; i < tiles.size();  i++){
    unique_ptr<Rectangle> tileMBR = getTileMBR(i);
    CollisionType collisionType = detectMBRCollision(mbr, *tileMBR);
    if(collisionType != COLLISION_NONE){
      CollisionTup collision;
      collision.collisionType = collisionType;
      collision.tile = tiles[i]->tile;
      collisions.push_back(collision);
    }
  }
  
  return collisions;
}

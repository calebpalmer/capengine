#include "tileset.h"

#include "filesystem.h"
#include "locator.h"
#include "CapEngineException.h"

#include <fstream>
#include <sstream>
#include <memory>
#include <boost/filesystem.hpp>

using namespace CapEngine;
using namespace std;

TileSet::TileSet(const string& configPath) {
  // test that configPath exists and throw exception if it doesn't
  if(!fileExists(configPath)){
    BOOST_THROW_EXCEPTION(CapEngineException(configPath + " is not a valid path"));
  }
  this->m_configFilePath = configPath;

  // read configFile loading
  ifstream configIn(configPath);
  string line;
  int linenum = 1;

  // parse headers
  while(configIn.good()){
    getline(configIn, line);
    string::size_type position;
    position = line.find("=");

    if(position == string::npos){
      stringstream errorMessage;
      errorMessage << "Unable to parse config file " << configPath << " at line " << linenum;
      BOOST_THROW_EXCEPTION(CapEngineException(errorMessage.str()));
    }

    string parameter = line.substr(0, position);
    string value = line.substr(position + 1,line.size() - position + 1);

    if(parameter == "tileset_path"){
      this->m_surfaceFilePath = value;

			// if it's relative, make it relative to the config file
			boost::filesystem::path surfacePath(m_surfaceFilePath);

			if(surfacePath.is_relative()){
				boost::filesystem::path configFilePath(m_configFilePath);
				surfacePath = configFilePath.parent_path() / surfacePath;
				this->m_surfaceFilePath = surfacePath.string();
				
				if(!boost::filesystem::exists(surfacePath))
					CAP_THROW(CapEngineException("Surface path not found: " + surfacePath.string()));
			}
    }
    else if(parameter == "tile_count"){
      stringstream temp(value);
      temp >> this->m_tileCount;
    }
    else if(parameter == "tile_height"){
      stringstream temp(value);
      temp >> this->m_tileHeight;
    }
    else if(parameter == "tile_width"){
      stringstream temp(value);
      temp >> this->m_tileWidth;
    }
    else if(parameter == "tiles"){
      break;
    }
    else{
      stringstream errorMessage;
      errorMessage << "Unknown parameter " << parameter;
      BOOST_THROW_EXCEPTION(CapEngineException(errorMessage.str()));
    }
  }
  // parse tiles
  while(configIn.good()){
    getline(configIn, line);
    if(line == ""){
      break;
    }
    Tile tile = parseTile(line);
    m_tiles.push_back(tile);
  }

  //// Load surface
	assert(Locator::videoManager != nullptr);
	m_pSurface = std::shared_ptr<SDL_Surface>(Locator::videoManager->loadSurface(m_surfaceFilePath), SDL_FreeSurface);
	assert(m_pSurface != nullptr);

  validate();
  configIn.close();
}

Tile TileSet::parseTile(const string& line){
  int x, y, width, height;
  std::size_t position, oldPosition;
  
  // parse x
  position = line.find(",");

  if(position == string::npos){
    stringstream errorMessage;
    errorMessage << "Unable to parse tile in config file " << m_configFilePath;
    BOOST_THROW_EXCEPTION(CapEngineException(errorMessage.str()));
  }
  
  string temp = line.substr(0, position);
  istringstream tempStream(temp);
  tempStream >> x;
  oldPosition = position + 1;

  // parse y 
  position = line.find(",", oldPosition);
 if(position == string::npos){
    stringstream errorMessage;
    errorMessage << "Unable to parse tile in config file " << m_configFilePath;
    BOOST_THROW_EXCEPTION(CapEngineException(errorMessage.str()));
  }
  temp = line.substr(oldPosition, position);
  tempStream.str(temp);
  tempStream.clear();
  tempStream >> y;
  oldPosition = position + 1;

  //parse width
  position = line.find(",", oldPosition);
 if(position == string::npos){
    stringstream errorMessage;
    errorMessage << "Unable to parse tile in config file " << m_configFilePath;
    BOOST_THROW_EXCEPTION(CapEngineException(errorMessage.str()));
  }
  temp = line.substr(oldPosition, position);
  tempStream.str(temp);
  tempStream.clear();
  tempStream >> width;
  oldPosition = position + 1;

  // parse height
  position = line.find(",", oldPosition);
 if(position == string::npos){
    stringstream errorMessage;
    errorMessage << "Unable to parse tile in config file " << m_configFilePath;
    BOOST_THROW_EXCEPTION(CapEngineException(errorMessage.str()));
  }
  temp = line.substr(oldPosition, position);
  tempStream.str(temp);
  tempStream.clear();
  tempStream >> height;

  // parse type
  int type;
  position = line.find(",", oldPosition);
 if(position == string::npos){
    stringstream errorMessage;
    errorMessage << "Unable to parse tile in config file " << m_configFilePath;
    BOOST_THROW_EXCEPTION(CapEngineException(errorMessage.str()));
  }
  temp = line.substr(oldPosition, position);
  tempStream.str(temp);
  tempStream.clear();
  tempStream >> type;

  Tile tile;
  tile.xpos = x;
  tile.ypos = y;
  tile.width = width;
  tile.height = height;
  tile.type = static_cast<TileType>(type);
  return tile;
}

void TileSet::validate(){
  if(m_tileCount != m_tiles.size()){
    BOOST_THROW_EXCEPTION(CapEngineException("Tiles read does not equal declared tile count"));
  }
  /*if(m_pSurface == nullptr){
    throw CapEngineException("Surface not loaded");
    }*/
}

bool TileSet::tileExists(unsigned int index){
  if(index >= m_tiles.size()){
    return false;
  }
  else{
    return true;
  }
}

Tile TileSet::getTile(unsigned int index){
  if(index >= m_tiles.size()){
    throw CapEngineException("Tile does not exist at index " + std::to_string(index));
  }
  else{
    return m_tiles[index];
  }
}

unsigned int TileSet::getTileSize() const{
  return m_tileWidth;
}


//! Gets the number of tiles in the tileset
/** 
 \return The number of tiles in the tileset
*/
unsigned int TileSet::getNumTiles() const{
	return m_tiles.size();
}


//! Get the surface with all the Tiles
/** 
 \return The surface
*/
std::shared_ptr<SDL_Surface> TileSet::getSurface(){
	return m_pSurface;
}


//! Gets the path to the config file for this tileset.
/** 
 \return The path to the config file.
*/
std::string TileSet::getConfigFilePath() const{
	return m_configFilePath;
}


//! Gets the path to the image file with the tiles.
/** 
 \return The path to the image file.
*/
std::string TileSet::getSurfaceFilePath() const{
	return m_surfaceFilePath;
}


//! Gets the tiles.
/** 
 \return The tiles.
*/
const std::vector<Tile>& TileSet::getTiles() const{
	return m_tiles;
}


//! Gets the width of tiles in this set.
/** 
 \return The width of tiles;
*/
unsigned int TileSet::getTileWidth() const{
	return m_tileWidth;
}

//! Gets the height of tiles in this set.
/** 
 \return The height of tiles;
*/
unsigned int TileSet::getTileHeight() const{
	return m_tileHeight;
}


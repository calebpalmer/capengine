#include "tileset.h"

#include <fstream>
#include <sstream>
#include <memory>

#include "filesystem.h"
#include "locator.h"
#include "CapEngineException.h"

using namespace CapEngine;
using namespace std;

TileSet::~TileSet(){
  if(surface != nullptr){
    videoManager->closeSurface(surface);
    surface = nullptr;
  }
}

TileSet::TileSet(const string& configPath) {
  // test that configPath exists and throw exception if it doesn't
  if(!fileExists(configPath)){
    BOOST_THROW_EXCEPTION(CapEngineException(configPath + " is not a valid path"));
  }
  this->configFilepath = configPath;

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
      this->surfaceFilepath = value;
    }
    else if(parameter == "tile_count"){
      stringstream temp(value);
      temp >> this->tileCount;
    }
    else if(parameter == "tile_height"){
      stringstream temp(value);
      temp >> this->tileHeight;
    }
    else if(parameter == "tile_width"){
      stringstream temp(value);
      temp >> this->tileWidth;
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
    tiles.push_back(tile);
  }

  //// Load surface
  videoManager = Locator::videoManager;
  if(videoManager){
    if(videoManager->initialized == false){
      BOOST_THROW_EXCEPTION(CapEngineException("VideoManager not initialized"));
    }
  
    surface = videoManager->loadSurface(surfaceFilepath);
  }
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
    errorMessage << "Unable to parse tile in config file " << configFilepath;
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
    errorMessage << "Unable to parse tile in config file " << configFilepath;
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
    errorMessage << "Unable to parse tile in config file " << configFilepath;
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
    errorMessage << "Unable to parse tile in config file " << configFilepath;
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
    errorMessage << "Unable to parse tile in config file " << configFilepath;
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
  if(tileCount != tiles.size()){
    BOOST_THROW_EXCEPTION(CapEngineException("Tiles read does not equal declared tile count"));
  }
  /*if(surface == nullptr){
    throw CapEngineException("Surface not loaded");
    }*/
}

bool TileSet::tileExists(unsigned int index){
  if(index >= tiles.size()){
    return false;
  }
  else{
    return true;
  }
}

Tile TileSet::getTile(unsigned int index){
  if(index >= tiles.size()){
    throw CapEngineException("Tile does not exist at index " + index);
  }
  else{
    return tiles[index];
  }
}

unsigned int TileSet::getTileSize() const{
  return tileWidth;
}

#include "map2d.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "CapEngineException.h"
#include "filesystem.h"
#include "logger.h"
#include "VideoManager.h"

using namespace CapEngine;
using namespace std;

Map2D::~Map2D(){
  if(surface != nullptr){
    VideoManager::getInstance().closeSurface(surface);
  }
  vector<TileTup*>::iterator iter;
  for(iter = tiles.begin(); iter != tiles.end(); iter++){
    delete *iter;
  }
}

Map2D::Map2D(const string mapConfigPath,  bool loadVideo) : tileSet(nullptr) {
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
      Logger::getInstance().log("Invalid Line", Logger::CWARNING);
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
      tileSet.reset(new TileSet(this->tileSetPath, loadVideo));
      setCurrentDir(prevPath);
    }
    else if(parameter == "tiles"){
      readTiles(configStream);
    }
    else{
      // log warning
      Logger& logger = Logger::getInstance();
      logger.log("Invalid parameter: " + parameter, Logger::CWARNING);
    }
      
  }
  configStream.close();

  if(loadVideo){
    // load and draw surface
    VideoManager& videoManager = VideoManager::getInstance();
    if(videoManager.initialized == false){
      throw CapEngineException("VideoManager not initialized");
    }
    surface = videoManager.createSurface(width, height);
    drawSurface();
  }
  ostringstream logString;
  logString << "loaded map from " << mapConfigPath << endl
	    << " using tileset " << tileSet->configFilepath << endl
	    << " with " << tiles.size() << " tiles loaded " << endl;
  Logger::getInstance().log(logString.str(), Logger::CDEBUG);
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

void Map2D::drawSurface(){
  unsigned int xRes, yRes = 0;
  VideoManager& videoManager = VideoManager::getInstance();
  if(videoManager.initialized == false){
    throw CapEngineException("VideoManager not initialized");
  }
  vector<TileTup*>::iterator iter;
  for(iter = tiles.begin(); iter != tiles.end(); iter++){
    if(xRes >= width){
      xRes = 0;
      yRes += (*iter)->tile->height;
    }
    videoManager.blitSurface(*(tileSet->surface), (*iter)->tile->xpos, (*iter)->tile->ypos, (*iter)->tile->width, (*iter)->tile->height, *surface, xRes, yRes);
    xRes += (*iter)->tile->width;
  }
  Logger::getInstance().log("Drew consolidate map surface", Logger::CDEBUG);
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

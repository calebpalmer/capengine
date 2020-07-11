#include "tileset.h"

#include "CapEngineException.h"
#include "filesystem.h"
#include "locator.h"
#include "logger.h"

#include <boost/filesystem.hpp>
#include <filesystem>
#include <fstream>
#include <memory>
#include <sstream>

using namespace std;

namespace CapEngine
{

namespace
{

// json schema peroperty names.
const char kSurfaceFileProperty[] = "imagePath";
const char kTileCountProperty[] = "numTiles";
const char kTileWidthProperty[] = "tileWidth";
const char kTileHeightProperty[] = "tileHeight";
const char kTileArrayProperty[] = "tiles";
const char kTilePositionProperty[] = "position";
const char kTilePositionXProperty[] = "x";
const char kTilePositionYProperty[] = "y";
const char kTilePositionWidthProperty[] = "width";
const char kTilePositionHeightProperty[] = "height";
const char kTileTypeProperty[] = "type";

//! Returns string representation of TileType
/**
 \param tileType
   \li The TileType
 \return
   \li The string representation of the TileType
*/
std::string tileTypeToString(TileType tileType)
{
  switch (tileType) {
  case TILE_NORMAL:
    return "Normal";
  case TILE_SOLID:
    return "Solid";
  default:
    return "Unknown";
  }
}

//! Converts string to TileType
/**
 \param tileType
   \li The string to convert.
 \return
   \li The TileType
*/
TileType stringToTileType(const std::string &tileType)
{
  if (tileType == tileTypeToString(TILE_NORMAL))
    return TILE_NORMAL;

  else if (tileType == tileTypeToString(TILE_SOLID))
    return TILE_SOLID;

  else
    return TILE_UNKNOWN;
}

} // namespace

TileSet::TileSet(const string &configPath)
{
  // test that configPath exists and throw exception if it doesn't
  if (!fileExists(configPath)) {
    BOOST_THROW_EXCEPTION(
        CapEngineException(configPath + " is not a valid path"));
  }

  // read configFile loading
  ifstream configIn(configPath);

  // try read json first
  jsoncons::json_decoder<jsoncons::json> decoder;
  jsoncons::json_reader reader(configIn, decoder);

  try {
    reader.read();
    jsoncons::json j = decoder.get_result();

    TileSet other(j);
    *this = std::move(other);

    this->m_configFilePath = configPath;
    this->loadSurface();
    this->validate();

    return;
  } catch (const jsoncons::parse_error &e) {
  }

  // otherwise use old method
  this->m_configFilePath = configPath;

  string line;
  int linenum = 1;

  // parse headers
  while (configIn.good()) {
    getline(configIn, line);
    string::size_type position;
    position = line.find("=");

    if (position == string::npos) {
      stringstream errorMessage;
      errorMessage << "Unable to parse config file " << configPath
                   << " at line " << linenum;
      BOOST_THROW_EXCEPTION(CapEngineException(errorMessage.str()));
    }

    string parameter = line.substr(0, position);
    string value = line.substr(position + 1, line.size() - position + 1);

    if (parameter == "tileset_path") {
      this->m_surfaceFilePath = value;

      // if it's relative, make it relative to the config file
      boost::filesystem::path surfacePath(m_surfaceFilePath);

      if (surfacePath.is_relative()) {
        boost::filesystem::path configFilePath(m_configFilePath);
        surfacePath = configFilePath.parent_path() / surfacePath;
        this->m_surfaceFilePath = surfacePath.string();

        if (!boost::filesystem::exists(surfacePath))
          CAP_THROW(CapEngineException("Surface path not found: " +
                                       surfacePath.string()));
      }
    } else if (parameter == "tile_count") {
      stringstream temp(value);
      temp >> this->m_tileCount;
    } else if (parameter == "tile_height") {
      stringstream temp(value);
      temp >> this->m_tileHeight;
    } else if (parameter == "tile_width") {
      stringstream temp(value);
      temp >> this->m_tileWidth;
    } else if (parameter == "tiles") {
      break;
    } else {
      stringstream errorMessage;
      errorMessage << "Unknown parameter " << parameter;
      BOOST_THROW_EXCEPTION(CapEngineException(errorMessage.str()));
    }
  }
  // parse tiles
  while (configIn.good()) {
    getline(configIn, line);
    if (line == "") {
      break;
    }
    Tile tile = parseTile(line);
    m_tiles.push_back(tile);
  }

  //// Load surface
  this->loadSurface();
  this->validate();
}

//! Constructor.
/**
 \param json
   \li The json to read the Tileset from.
*/
TileSet::TileSet(const jsoncons::json &json)
{
  m_surfaceFilePath = json[kSurfaceFileProperty].as<std::string>();
  ;
  m_tileCount = json[kTileCountProperty].as<int>();
  m_tileWidth = json[kTileWidthProperty].as<int>();
  m_tileHeight = json[kTileHeightProperty].as<int>();

  // get the tiles
  for (auto &&jsonTile : json[kTileArrayProperty].array_range()) {
    try {
      Tile tile;
      tile.type =
          stringToTileType(jsonTile[kTileTypeProperty].as<std::string>());

      jsoncons::json tilePosition = jsonTile[kTilePositionProperty];
      tile.xpos = tilePosition[kTilePositionXProperty].as<int>();
      tile.ypos = tilePosition[kTilePositionYProperty].as<int>();
      tile.width = tilePosition[kTilePositionWidthProperty].as<int>();
      tile.height = tilePosition[kTilePositionHeightProperty].as<int>();

      m_tiles.push_back(tile);
    }

    catch (jsoncons::json_exception &e) {
      CAP_LOG(Locator::logger, e, Logger::CWARNING);
    }
  }
}

//! Copy constructor
/**
 \param other
   \li The other TileSet to copy from.
*/
TileSet::TileSet(const TileSet &other) { this->copy(other); }

//! Copy assignment operator.
TileSet &TileSet::operator=(const TileSet &other)
{
  this->copy(other);
  return *this;
}

Tile TileSet::parseTile(const string &line)
{
  int x, y, width, height;
  std::size_t position, oldPosition;

  // parse x
  position = line.find(",");

  if (position == string::npos) {
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
  if (position == string::npos) {
    stringstream errorMessage;
    errorMessage << "Unable to parse tile in config file " << m_configFilePath;
    BOOST_THROW_EXCEPTION(CapEngineException(errorMessage.str()));
  }
  temp = line.substr(oldPosition, position);
  tempStream.str(temp);
  tempStream.clear();
  tempStream >> y;
  oldPosition = position + 1;

  // parse width
  position = line.find(",", oldPosition);
  if (position == string::npos) {
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
  if (position == string::npos) {
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
  if (position == string::npos) {
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

void TileSet::validate()
{
  if (m_tileCount != m_tiles.size()) {
    BOOST_THROW_EXCEPTION(
        CapEngineException("Tiles read does not equal declared tile count"));
  }
  /*if(m_pSurface == nullptr){
    throw CapEngineException("Surface not loaded");
    }*/
}

bool TileSet::tileExists(unsigned int index) const
{
  if (index >= m_tiles.size()) {
    return false;
  } else {
    return true;
  }
}

Tile TileSet::getTile(unsigned int index) const
{
  if (index >= m_tiles.size()) {
    throw CapEngineException("Tile does not exist at index " +
                             std::to_string(index));
  } else {
    return m_tiles[index];
  }
}

unsigned int TileSet::getTileSize() const { return m_tileWidth; }

//! Gets the number of tiles in the tileset
/**
 \return The number of tiles in the tileset
*/
unsigned int TileSet::getNumTiles() const { return m_tiles.size(); }

//! Get the surface with all the Tiles
/**
 \return The surface
*/
std::shared_ptr<SDL_Surface> TileSet::getSurface() { return m_pSurface; }

//! Gets the path to the config file for this tileset.
/**
 \return The path to the config file.
*/
std::string TileSet::getConfigFilePath() const { return m_configFilePath; }

//! Gets the path to the image file with the tiles.
/**
 \return The path to the image file.
*/
std::string TileSet::getSurfaceFilePath() const { return m_surfaceFilePath; }

//! Gets the tiles.
/**
 \return The tiles.
*/
const std::vector<Tile> &TileSet::getTiles() const { return m_tiles; }

//! Gets the width of tiles in this set.
/**
 \return The width of tiles;
*/
unsigned int TileSet::getTileWidth() const { return m_tileWidth; }

//! Gets the height of tiles in this set.
/**
 \return The height of tiles;
*/
unsigned int TileSet::getTileHeight() const { return m_tileHeight; }

//! Gets json representation of tileset
/**
 \return
   \li The json repr of the TileSet.
*/
jsoncons::json TileSet::json() const
{
  jsoncons::json json;
  json.insert_or_assign(kSurfaceFileProperty, m_surfaceFilePath);
  json.insert_or_assign(kTileCountProperty, m_tileCount);
  json.insert_or_assign(kTileWidthProperty, m_tileWidth);
  json.insert_or_assign(kTileHeightProperty, m_tileHeight);

  jsoncons::json::array tiles(m_tiles.size());
  for (size_t i = 0; i < m_tiles.size(); i++) {

    const Tile &tile = m_tiles[i];
    jsoncons::json jsonTile;

    jsoncons::json position;
    position.insert_or_assign(kTilePositionXProperty, tile.xpos);
    position.insert_or_assign(kTilePositionYProperty, tile.ypos);
    position.insert_or_assign(kTilePositionWidthProperty, tile.width);
    position.insert_or_assign(kTilePositionHeightProperty, tile.height);
    jsonTile.insert_or_assign(kTilePositionProperty, std::move(position));

    jsonTile.insert_or_assign(kTileTypeProperty, tileTypeToString(tile.type));

    tiles[i] = std::move(jsonTile);
  }

  json.insert_or_assign(kTileArrayProperty, std::move(tiles));
  return json;
}

//! Saves the tileset.
/**
 \param filepath
   \li The filepath to where it will be saved.
*/
void TileSet::save(const std::string &filepath)
{
  std::ofstream f(filepath);

  jsoncons::json_serializing_options options;
  options.object_array_split_lines(jsoncons::line_split_kind::new_line);
  f << jsoncons::pretty_print(this->json(), options);
}

//! Copies from another TileSet
/**
 \param other
   \li The TileSet to copy from.
*/
void TileSet::copy(const TileSet &other)
{
  this->m_configFilePath = other.m_configFilePath;
  this->m_surfaceFilePath = other.m_surfaceFilePath;
  this->m_tiles = other.m_tiles;
  this->m_tileCount = other.m_tileCount;
  this->m_tileWidth = other.m_tileWidth;
  this->m_tileHeight = other.m_tileHeight;
}

// Load the surface
void TileSet::loadSurface()
{
  assert(Locator::videoManager != nullptr);
  m_pSurface = std::shared_ptr<SDL_Surface>(
      Locator::videoManager->loadSurface(m_surfaceFilePath), SDL_FreeSurface);
  assert(m_pSurface != nullptr);
}

} // namespace CapEngine

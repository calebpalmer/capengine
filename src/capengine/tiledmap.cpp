#include "tiledmap.h"

#include <boost/throw_exception.hpp>
#include <exception>
#include <filesystem>
#include <fstream>
#include <sstream>

#include "CapEngineException.h"
#include "defer.h"
#include "locator.h"
#include "tiledtilelayer.h"
#include "tiledtileset.h"

namespace CapEngine
{

TiledMap::TiledMap(const jsoncons::json &in_json,
				   std::optional<std::filesystem::path> in_path)
	: m_path(std::move(in_path)),
	  m_texture(getNullTexturePtr()),
	  m_tilesets(std::make_shared<std::vector<TiledTileset>>())
{
	this->loadJson(in_json);

	// create the texture
	int textureWidth{m_width * m_tileWidth};
	int textureHeight{m_width * m_tileHeight};
	m_texture = Locator::getVideoManager().createTexturePtr(textureWidth,
															textureHeight);
}

TiledMap::TiledMap(const std::filesystem::path &in_mapPath)
	: m_path(in_mapPath),
	  m_texture(getNullTexturePtr()),
	  m_tilesets(std::make_shared<std::vector<TiledTileset>>())
{
	std::ifstream t(in_mapPath, std::ios::in);
	std::string str((std::istreambuf_iterator<char>(t)),
					std::istreambuf_iterator<char>());

	std::ifstream f(in_mapPath, std::ios::in);
	auto mapData = jsoncons::json::parse(f);
	this->loadJson(mapData);

	// create the texture
	int textureWidth{m_width * m_tileWidth};
	int textureHeight{m_width * m_tileHeight};
	m_texture = Locator::getVideoManager().createTexturePtr(textureWidth,
															textureHeight);
}

void TiledMap::loadJson(const jsoncons::json &in_json)
{
	m_tileHeight = in_json["tileheight"].as<int>();
	m_tileWidth = in_json["tilewidth"].as<int>();
	m_width = in_json["width"].as<int>();
	m_height = in_json["height"].as<int>();

	// load tilesets
	assert(m_tilesets);
	m_tilesets->reserve(in_json.count("tilesets"));
	for (auto &&i : in_json["tilesets"].array_range()) {
		if (i.contains("source")) {
			std::string tilesetPath = i["source"].as<std::string>();
			if (m_path) {
				tilesetPath = m_path->parent_path() / tilesetPath;
			}

			if (!i.contains("firstgid")) {
				BOOST_THROW_EXCEPTION(
					CapEngineException{"tileset missing firstGid property"});
			}
			m_tilesets->push_back(
				TiledTileset::create(tilesetPath, i.at("firstgid").as<int>()));
		} else {
			if (!i.contains("firstgid")) {
				BOOST_THROW_EXCEPTION(
					CapEngineException{"tileset missing firstGid property"});
			}
			m_tilesets->push_back(
				TiledTileset(i, i["firstgid"].as<int>(), this->m_path));
		}
	}

	// load layers
	for (auto &&i : in_json["layers"].array_range()) {
		if (i.at("type").as<std::string>() == "tilelayer") {
			m_layers.emplace_back(i, m_tilesets, m_tileWidth,
											  m_tileHeight, m_width, m_height);
		}
	}
}

int TiledMap::tileHeight() const { return m_tileHeight; }
int TiledMap::tileWidth() const { return m_tileWidth; }
int TiledMap::width() const { return m_width; }
int TiledMap::height() const { return m_height; }

const std::vector<TiledTileset> &TiledMap::tilesets() const
{
	return *m_tilesets;
}
const std::vector<TiledTileLayer> &TiledMap::layers() const { return m_layers; }

void TiledMap::render()
{
	assert(m_texture != nullptr);

	for (auto &&layer : m_layers) {
		auto *texture = layer.texture();
		assert(texture != nullptr);

		SDL_Rect rect{0, 0, m_width * m_tileWidth, m_height * m_tileHeight};
		Locator::getVideoManager().drawTexture(m_texture.get(), texture, rect,
											   rect);
	}
}

Texture *TiledMap::texture() { return m_texture.get(); }
}  // namespace CapEngine

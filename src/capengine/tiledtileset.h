#ifndef CAPENGINE_TILEDTILESET_H
#define CAPENGINE_TILEDTILESET_H

#include "captypes.h"

#include <filesystem>
#include <jsoncons/json.hpp>
#include <optional>
#include <string>

namespace CapEngine {

class TiledTileset final
{
   public:
	explicit TiledTileset(
		const jsoncons::json &in_data, int in_firstGid,
		std::optional<std::filesystem::path> in_path = std::nullopt);
	static TiledTileset create(std::filesystem::path in_path, int in_firstGid);

	[[nodiscard]] const std::string &image() const;
	[[nodiscard]] int firstGid() const;
	[[nodiscard]] int imageHeight() const;
	[[nodiscard]] int imageWidth() const;
	[[nodiscard]] int tileHeight() const;
	[[nodiscard]] int tileWidth() const;
	void loadTexture();
	std::optional<Texture *> texture();

private:
	std::optional<std::filesystem::path> m_path;
	std::string m_image;
	int m_firstGid{0};
	int m_imageHeight{0};
	int m_imageWidth{0};
	int m_tileHeight{0};
	int m_tileWidth{0};
	TexturePtr m_texture;
};
} // namespace CapEngine

#endif /* CAPENGINE_TILEDTILESET_H */

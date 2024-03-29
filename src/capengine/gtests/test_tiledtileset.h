#include <capengine/CapEngineException.h>
#include <capengine/captypes.h>
#include <capengine/locator.h>
#include <capengine/tiledtileset.h>
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <jsoncons/json.hpp>

namespace CapEngine::testing {

TEST(TiledTilesetTest, TestConstructor)
{
	std::filesystem::path mapPath =
		CapEngine::testing::getTestFilePath() / "tiled" / "tileset.tsj";
	std::ifstream f(mapPath);
	ASSERT_TRUE(f);
	jsoncons::json json = jsoncons::json::parse(f);
	CapEngine::TiledTileset tileset{json, 1};

	ASSERT_EQ("tileset.png", tileset.image());
	ASSERT_EQ(480, tileset.imageWidth());
	ASSERT_EQ(320, tileset.imageHeight());
	ASSERT_EQ(1, tileset.firstGid());
}

TEST(TiledTilesetTest, TestLoadTexture)
{
	std::filesystem::path mapPath =
		CapEngine::testing::getTestFilePath() / "tiled" / "tileset.tsj";
	auto tileset = CapEngine::TiledTileset::create(mapPath, 1);

	ASSERT_EQ(std::nullopt, tileset.texture());

	ASSERT_NO_THROW(tileset.loadTexture());

	auto texture = tileset.texture();
	ASSERT_EQ(480, CapEngine::Locator::videoManager->getTextureWidth(*texture));
	ASSERT_EQ(320,
			  CapEngine::Locator::videoManager->getTextureHeight(*texture));
	ASSERT_EQ(1, tileset.firstGid());
}

} // namespace CapEngine::Testing

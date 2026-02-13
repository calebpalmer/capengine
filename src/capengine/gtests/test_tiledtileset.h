#include <capengine/CapEngineException.h>
#include <capengine/captypes.h>
#include <capengine/locator.h>
#include <capengine/tiledtileset.h>
#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <jsoncons/json.hpp>

#include "testutils.h"

namespace CapEngine::testing {

TEST(TiledTilesetTest, TestConstructor)
{
    std::filesystem::path mapPath = CapEngine::testing::getTestFilePath() / "tiled" / "tileset.tsj";
    std::ifstream f(mapPath);
    ASSERT_TRUE(f);
    jsoncons::json json = jsoncons::json::parse(f);
    CapEngine::TiledTileset tileset{"test.tmj", json, 1};

    ASSERT_EQ("test.tmj", tileset.name());
    ASSERT_EQ("tileset.png", tileset.image());
    ASSERT_EQ(480, tileset.imageWidth());
    ASSERT_EQ(320, tileset.imageHeight());
    ASSERT_EQ(1, tileset.firstGid());
}

TEST(TiledTilesetTest, TestLoadTexture)
{
    std::filesystem::path tilesetPath = CapEngine::testing::getTestFilePath() / "tiled" / "tileset.tsj";
    auto tileset = CapEngine::TiledTileset::create("test.tmj", tilesetPath, 1);

    ASSERT_NO_THROW(tileset.loadTexture());

    auto texture = tileset.texture();
    ASSERT_EQ(480, CapEngine::Locator::videoManager->getTextureWidth(*texture));
    ASSERT_EQ(320, CapEngine::Locator::videoManager->getTextureHeight(*texture));
    ASSERT_EQ(1, tileset.firstGid());
}

TEST(TiledTilesetTest, TestTileRect)
{
    std::filesystem::path mapPath = CapEngine::testing::getTestFilePath() / "tiled" / "tileset.tsj";
    auto tileset = CapEngine::TiledTileset::create("test.tmj", mapPath, 0);

    ASSERT_EQ(Rectangle(0, 0, 16, 16), tileset.tileRect(0));
    ASSERT_EQ(Rectangle(16, 0, 16, 16), tileset.tileRect(1));
    ASSERT_EQ(Rectangle(32, 0, 16, 16), tileset.tileRect(2));
}

}  // namespace CapEngine::testing

#include <capengine/tiledtilelayer.h>
#include <capengine/tiledtileset.h>

#include <gtest/gtest.h>
#include <jsoncons/json.hpp>
#include <memory>

TEST(TiledTileLayerTest, TestConstructor)
{
    jsoncons::json j = jsoncons::json::parse(R"(
											  {
												"data":[1, 1,
														1, 31],
												"height":2,
												"id":1,
												"name":"Tile Layer 1",
												"opacity":1,
												"type":"tilelayer",
												"visible":true,
												"width":2,
												"x":0,
												"y":0
											  }
)");

    CapEngine::TiledTileLayer layer{
        j,   std::make_shared<std::vector<CapEngine::TiledTileset>>(),
        16,  16,
        640, 480};
    ASSERT_EQ(2, layer.width());
    ASSERT_EQ(2, layer.height());
    ASSERT_EQ(0, layer.x());
    ASSERT_EQ(0, layer.y());
    ASSERT_EQ(true, layer.visible());
    std::vector<unsigned int> expectedData{1, 1, 1, 31};
    ASSERT_EQ(expectedData, layer.data());
}

TEST(TiledTileLayerTest, TestGetGlobalTileInfo)
{
    // x flip only
    unsigned int data{0x80000000};
    auto tileInfo = CapEngine::getGlobalTileInfo(data);
    ASSERT_EQ(true, tileInfo.xFlip);
    ASSERT_EQ(false, tileInfo.yFlip);
    ASSERT_EQ(false, tileInfo.diagFlip);

    // y flip only
    data = 0x40000000;
    tileInfo = CapEngine::getGlobalTileInfo(data);
    ASSERT_EQ(false, tileInfo.xFlip);
    ASSERT_EQ(true, tileInfo.yFlip);
    ASSERT_EQ(false, tileInfo.diagFlip);

    // diag flip only
    data = 0x20000000;
    tileInfo = CapEngine::getGlobalTileInfo(data);
    ASSERT_EQ(false, tileInfo.xFlip);
    ASSERT_EQ(false, tileInfo.yFlip);
    ASSERT_EQ(true, tileInfo.diagFlip);

    // x and y flip
    data = 0xC0000000;
    tileInfo = CapEngine::getGlobalTileInfo(data);
    ASSERT_EQ(true, tileInfo.xFlip);
    ASSERT_EQ(true, tileInfo.yFlip);
    ASSERT_EQ(false, tileInfo.diagFlip);

    // x and diag flip
    data = 0xA0000000;
    tileInfo = CapEngine::getGlobalTileInfo(data);
    ASSERT_EQ(true, tileInfo.xFlip);
    ASSERT_EQ(false, tileInfo.yFlip);
    ASSERT_EQ(true, tileInfo.diagFlip);

    // y and diag flip
    data = 0x60000000;
    tileInfo = CapEngine::getGlobalTileInfo(data);
    ASSERT_EQ(false, tileInfo.xFlip);
    ASSERT_EQ(true, tileInfo.yFlip);
    ASSERT_EQ(true, tileInfo.diagFlip);

    // x, y, and diag flip
    // x and diag flip
    data = 0xE0000000;
    tileInfo = CapEngine::getGlobalTileInfo(data);
    ASSERT_EQ(true, tileInfo.xFlip);
    ASSERT_EQ(true, tileInfo.yFlip);
    ASSERT_EQ(true, tileInfo.diagFlip);

    // test with global tile id
    data = 0x80000000 | 1;
    tileInfo = CapEngine::getGlobalTileInfo(data);
    ASSERT_EQ(true, tileInfo.xFlip);
    ASSERT_EQ(false, tileInfo.yFlip);
    ASSERT_EQ(false, tileInfo.diagFlip);
    ASSERT_EQ(1, tileInfo.globalTileId);

    // test with global tile id
    data = 0xE0000000 | 255;
    tileInfo = CapEngine::getGlobalTileInfo(data);
    ASSERT_EQ(true, tileInfo.xFlip);
    ASSERT_EQ(true, tileInfo.yFlip);
    ASSERT_EQ(true, tileInfo.diagFlip);
    ASSERT_EQ(255, tileInfo.globalTileId);
}

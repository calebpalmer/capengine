#include <capengine/tiledmap.h>

#include <fstream>

#include "gtest/gtest.h"
#include "testutils.h"

namespace CapEngine::testing
{
TEST(TiledMapTest, TestConstructor)
{
    std::filesystem::path mapPath =
        CapEngine::testing::getTestFilePath() / "tiled" / "testmap.json";
    CapEngine::TiledMap map(mapPath);
    ASSERT_EQ(16, map.tileWidth());
    ASSERT_EQ(16, map.tileHeight());
    ASSERT_EQ(2, map.width());
    ASSERT_EQ(2, map.height());
    ASSERT_EQ(1, map.tilesets().size());
    ASSERT_EQ(1, map.layers().size());

    auto const& objectGroups = map.objectGroups();
    ASSERT_EQ(1, objectGroups.size());
}
}  // namespace CapEngine::testing

#include "testutils.h"
#include "gtest/gtest.h"

#include <capengine/tiledmap.h>
#include <fstream>

TEST(TiledMapTest, TestConstructor) {
  std::filesystem::path mapPath = CapEngine::Testing::getTestFilePath() / "tiled" / "testmap.json";
  CapEngine::TiledMap map(mapPath);
  ASSERT_EQ(16, map.tileWidth());
  ASSERT_EQ(16, map.tileHeight());
  ASSERT_EQ(2, map.width());
  ASSERT_EQ(2, map.height());
  ASSERT_EQ(1, map.tilesets().size());
  ASSERT_EQ(1, map.layers().size());
}

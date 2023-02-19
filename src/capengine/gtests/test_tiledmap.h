#include "testutils.h"
#include "gtest/gtest.h"

#include <capengine/tiledmap.h>
#include <fstream>

// test constructor the takes string
TEST(TiledMapTest, TestConstructorStringData) {
  const std::string mapData{
      R"(
	   { "id": "blah" }
	   )"};

  CapEngine::TiledMap map{mapData};
  const std::string returnedMapData{map.getMapData()};

  ASSERT_EQ(mapData, returnedMapData);
}

// Test constructor that takes path
TEST(TiledMapTest, TestConstructorFileSystemPath) {
  const std::string mapData{"{ \"id\": \"blah\" }"};

  CapEngine::Testing::TempFile tempFile;

  {
    std::ofstream f{tempFile.getFilePath()};
    f << mapData;
  }

  CapEngine::TiledMap map{tempFile.getFilePath()};
  const std::string returnedMapData{map.getMapData()};

  ASSERT_EQ(mapData, returnedMapData);
}

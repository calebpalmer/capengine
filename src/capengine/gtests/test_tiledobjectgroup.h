#include <capengine/tiledobjectgroup.h>
#include <gtest/gtest.h>

#include <jsoncons/json.hpp>
#include <memory>

#include "capengine/tiledtileset.h"

namespace CapEngine::testing {

namespace {
const jsoncons::json kTestObjects = jsoncons::json::parse(R"(
{
"objects":
  [
    {
      "class":"superclass",
      "height": 120.0,
      "id":1,
      "name":"superobject",
      "rotation":0,
      "visible":true,
      "width":150.0,
      "x":103.2,
          "y":244.4,
          "properties":[
              {
                  "name":"capengine-font-ttf",
                      "type":"string",
                      "value":"tahoma.ttf"
              }
          ]
    },
    {
      "class":"superclass",
      "height": 120.0,
      "id":1,
      "name":"superobject2",
      "rotation":0,
      "visible":true,
      "width":150.0,
      "x":203.2,
      "y":44.4
    },
    {
      "class":"subclass",
      "height": 100.0,
      "id":1,
      "name":"sadsubobject",
      "rotation":0,
      "visible":true,
      "width":80.0,
      "x":10.0,
      "y":90.0
    }
  ]
}
)");
}

TEST(TiledObjectGroupTest, TestObjectFromJson)
{
    jsoncons::json objectJson = jsoncons::json::parse(R"(
{
  "class":"superclass",
  "height": 120.0,
  "id":1,
  "name":"superobject",
  "rotation":0,
  "visible":true,
  "width":150.0,
  "x":103.2,
  "y":244.4
})");

    TiledObjectGroup::Object object{objectJson};
    ASSERT_EQ(object.class_, "superclass");
    ASSERT_EQ(object.name, "superobject");
    ASSERT_EQ(object.visible, true);
    ASSERT_EQ(object.width, 150.0);
    ASSERT_EQ(object.height, 120.0);
    ASSERT_EQ(object.x, 103.2);
    ASSERT_EQ(object.y, 244.4);
}

TEST(TiledObjectGroupTest, TestObjectGroupFromJson)
{
    std::vector<std::unique_ptr<TiledTileset>> tilesets;
    TiledObjectGroup objectGroup{kTestObjects, 64, 64, tilesets};

    auto const& objects = objectGroup.objects();
    ASSERT_EQ(3, objects.size());

    auto const& object = objects.find("superobject");
    ASSERT_NE(objects.end(), object);

    auto const& object2 = objects.find("superobject2");
    ASSERT_NE(objects.end(), object);

    auto const& object3 = objects.find("sadsubobject");
    ASSERT_NE(objects.end(), object);

    auto nonConstObjects = objectGroup.objects();
}

TEST(TileOBjectGroupTest, TestFindObjectByName)
{
    std::vector<std::unique_ptr<TiledTileset>> tilesets;
    TiledObjectGroup objectGroup{kTestObjects, 64, 64, tilesets};

    auto maybeObject = objectGroup.objectByName("superobject");
    ASSERT_TRUE(maybeObject.has_value());
    auto object = maybeObject.value();
    ASSERT_EQ(object.class_, "superclass");
    ASSERT_EQ(object.name, "superobject");
    ASSERT_EQ(object.visible, true);
    ASSERT_EQ(object.width, 150.0);
    ASSERT_EQ(object.height, 120.0);
    ASSERT_EQ(object.x, 103.2);
    ASSERT_EQ(object.y, 244.4);

    auto maybeObject2 = objectGroup.objectByName("superobject2");
    ASSERT_TRUE(maybeObject2.has_value());
    auto object2 = maybeObject2.value();
    ASSERT_EQ(object2.class_, "superclass");
    ASSERT_EQ(object2.name, "superobject2");
    ASSERT_EQ(object2.visible, true);
    ASSERT_EQ(object2.width, 150.0);
    ASSERT_EQ(object2.height, 120.0);
    ASSERT_EQ(object2.x, 203.2);
    ASSERT_EQ(object2.y, 44.4);

    auto maybeObject3 = objectGroup.objectByName("sadsubobject");
    ASSERT_TRUE(maybeObject3.has_value());
    auto object3 = maybeObject3.value();
    ASSERT_EQ(object3.class_, "subclass");
    ASSERT_EQ(object3.name, "sadsubobject");
    ASSERT_EQ(object3.visible, true);
    ASSERT_EQ(object3.width, 80.0);
    ASSERT_EQ(object3.height, 100.0);
    ASSERT_EQ(object3.x, 10.0);
    ASSERT_EQ(object3.y, 90.0);

    // test object not found
    auto maybeObject4 = objectGroup.objectByName("notfound");
    ASSERT_FALSE(maybeObject4.has_value());
}

}  // namespace CapEngine::testing

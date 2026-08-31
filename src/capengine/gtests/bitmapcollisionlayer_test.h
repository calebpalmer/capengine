#include <gtest/gtest.h>
#include <memory>

#include "../bitmapcollisionlayer.h"
#include "../locator.h"
#include "../collision.h"
#include "../scanconvert.h"
#include "capengine/asset_manager.h"
#include "capengine/gameobject.h"
#include "../boxcollider.h"
#include "capengine/vector.h"

namespace CapEngine::testing {

TEST(BitmapCollisionLayerTest, CheckCollisionsNoCollisions)
{
    auto& videoManager = Locator::getVideoManager();
    SurfacePtr surface = videoManager.createSurfacePtr(20, 20);
    fillRectangle(surface.get(), Rect{0, 0, 20, 20}, Colour{255, 255, 255}, CoordinateSystem::YUP);
    fillRectangle(surface.get(), Rect{5, 0, 5, 10}, Colour{0, 0, 0}, CoordinateSystem::YUP);

    AssetManager assetManager{};
    Locator::assetManager = &assetManager;
    const int bitmapAssetId = 1000;
    assetManager.loadSurface(bitmapAssetId, surface.get());

    BitmapCollisionLayer bcl{bitmapAssetId, Rectangle{0, 0, 20, 20}};
    GameObject obj{};
    auto boxCollider = std::make_shared<BoxCollider>(Rectangle{0, 0, 2, 2});
    obj.addComponent(boxCollider);

    Layer::CollisionType_t collisionType = bcl.checkCollisions(obj);
    ASSERT_EQ(0, collisionType.size());
}

TEST(BitmapCollisionLayerTest, CheckCollisionsCollison)
{
    auto& videoManager = Locator::getVideoManager();
    SurfacePtr surface = videoManager.createSurfacePtr(20, 20);
    fillRectangle(surface.get(), Rect{0, 0, 20, 20}, Colour{255, 255, 255}, CoordinateSystem::YUP);
    fillRectangle(surface.get(), Rect{5, 0, 5, 10}, Colour{0, 0, 0}, CoordinateSystem::YUP);

    AssetManager assetManager{};
    Locator::assetManager = &assetManager;
    const int bitmapAssetId = 1000;
    assetManager.loadSurface(bitmapAssetId, surface.get());

    BitmapCollisionLayer bcl{bitmapAssetId, Rectangle{0, 0, 20, 20}};
    GameObject obj{};
    obj.setPosition(Vector{5.0, 0.0});
    auto boxCollider = std::make_shared<BoxCollider>(Rectangle{0, 0, 2, 2});
    obj.addComponent(boxCollider);

    Layer::CollisionType_t collisionType = bcl.checkCollisions(obj);
    ASSERT_GE(collisionType.size(), 0);
}

}  // namespace CapEngine::testing

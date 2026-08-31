#include <gtest/gtest.h>
#include <fstream>

#include "../asset_manager.h"
#include "capengine/CapEngineException.h"
#include "testutils.h"
#include "../locator.h"

namespace CapEngine::testing {

TEST(AssetManagerTests, AssetManagetTest)
{
    std::filesystem::path testAssetFilePath = CapEngine::testing::getTestFilePath() / "assetmanager" / "assets.json";
}

TEST(AssetManagerTests, TestDefaultConstructor)
{
    CapEngine::AssetManager assetManager;
}

TEST(AssetManagerTests, TestJsonConstructor)
{
    auto jsonAssetFile = getTestFilePath() / "assetmanager" / "assets.json";
    std::ifstream is{jsonAssetFile};
    auto jsonAssets = jsoncons::json::parse(is);

    CapEngine::AssetManager assetManager{Locator::getVideoManager(), Locator::getSoundPlayer(), jsonAssets,
                                         getTestFilePath() / "assetmanager"};

    auto image = assetManager.getImage(1);
    ASSERT_NE(image.texture, nullptr);
}

TEST(AssetManagerTests, TestLoadSurface)
{
    AssetManager assetManager{};

    SurfacePtr surface = Locator::getVideoManager().createSurfacePtr(20, 20);
    const int id = 100;
    assetManager.loadSurface(id, surface.get());

    ASSERT_TRUE(assetManager.imageExists(id));
    auto image = assetManager.getImage(id);
    ASSERT_NE(nullptr, image.texture);

    ASSERT_THROW(assetManager.loadSurface(id, surface.get()), CapEngineException);
}

}  // namespace CapEngine::testing

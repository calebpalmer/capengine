#include "test_tiledmap.h"
#include "test_tiledtilelayer.h"
#include "test_tiledtileset.h"

#include <capengine/game_management.h>
#include <gtest/gtest.h>

class Environment : public ::testing::Environment {
public:
  Environment() { CapEngine::init(CapEngine::WindowParams{"Tests", 0, 0, 0, false, false, false, false, "Tests"}); }
  ~Environment() override { CapEngine::end(); }

  // Override this to define how to set up the environment.
  void SetUp() override {}

  // Override this to define how to tear down the environment.
  void TearDown() override {}
};

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  auto env = *AddGlobalTestEnvironment(new Environment);
  return RUN_ALL_TESTS();
}

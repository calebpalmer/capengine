#ifndef TESTENVIRONMENT_H
#define TESTENVIRONMENT_H

#include "../VideoManager.h"
#include "../game_management.h"

#include "gtest/gtest.h"

namespace CapEngine::testing {

class TestEnvironment : public ::testing::Environment {
public:
	TestEnvironment();
	~TestEnvironment() override;

  void SetUp() override {}
  void TearDown() override {}

  static TestEnvironment* instance();
  [[nodiscard]] uint32_t getWindowId() const { return m_windowId; }

private:
  static TestEnvironment* gTestEnvironment;
  uint32_t m_windowId = 0;
};
}  // namespace CapEngine::testing

#endif // TESTENVIRONMENT_H

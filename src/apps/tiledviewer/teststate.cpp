#include "teststate.h"

namespace Game {

namespace fs = std::filesystem;

TestState::TestState(fs::path in_mapPath) : m_mapPath(std::move(in_mapPath)) {}

void TestState::render() {}
void TestState::update(double ms) {}

} // namespace Game

#ifndef GAME_TESTSTATE_H
#define GAME_TESTSTATE_H

#include <capengine/gamestate.h>
#include <filesystem>
#include <string>

namespace Game {
class TestState : public CapEngine::GameState {
  public:
    TestState(std::filesystem::path in_mapPath);
    ~TestState() override{};
    void render() override;
    void update(double ms) override;

  private:
    std::filesystem::path m_mapPath;
};

} // namespace Game

#endif /* GAME_TESTSTATE_H */

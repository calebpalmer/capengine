#ifndef RUNNER_H
#define RUNNER_H

#include "IEventSubscriber.h"
#include "control.h"
#include "gamestate.h"
#include "timestep.h"

#include <SDL2/SDL.h>
#include <memory>
#include <vector>

namespace CapEngine
{

namespace UI
{
class Widget;
}

class Runner : public IEventSubscriber
{
  public:
    static Runner &getInstance();
    void switchState(std::shared_ptr<GameState> pGameState);
    void popState();
    void pushState(std::shared_ptr<GameState> pGameState);
    std::shared_ptr<GameState> peekState();
    void receiveEvent(const SDL_Event event,
                      CapEngine::Time *time) override; // IEventSubscriber
    void loop();
    void end();

    void setDefaultQuitEvents(bool enabled = true);

  protected:
    Runner();
    Runner(const Runner &);
    Runner &operator=(const Runner &);
    void exit();
    void render(double frameFactor);
    void update();

    static Runner *s_pRunner;
    std::vector<std::shared_ptr<GameState>> m_gameStates;
    std::vector<std::shared_ptr<UI::Widget>> m_widgets;
    std::vector<std::shared_ptr<GameState>> m_stateTrash;
    bool m_quit;
    bool m_showFPS;
    TimeStep m_timeStep;
    double m_msPerUpdate; // 16.67 = 60fps, 33.33 = 30fps
    //! flag indicating if exiting on window close/q keypress is enabled.
    bool m_defaultQuitEventsEnabled = true;
};

} // namespace CapEngine

#endif // RUNNER_H

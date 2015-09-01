#ifndef RUNNER_H
#define RUNNER_H

#include "gamestate.h"
#include "IEventSubscriber.h"
#include "timestep.h"

#include <SDL2/SDL.h>
#include <memory>
#include <vector>

namespace CapEngine{

  class Runner : public IEventSubscriber {
  public:
    static Runner& getInstance();
    void switchState(std::unique_ptr<GameState> pGameState);
    void popState();
    void pushState(std::unique_ptr<GameState> pGameState);
    void receiveEvent(const SDL_Event event, CapEngine::Time* time);  // IEventSubscriber
    void loop();
    void end();
    
  protected:
    Runner();
    Runner(const Runner&);
    Runner& operator=(const Runner&);
    void exit();
    void render(double frameFactor);
    void update();

    static Runner* s_pRunner;
    std::vector< std::unique_ptr<GameState> > m_gameStates;
    bool m_quit;
    bool m_showFPS;
    TimeStep m_timeStep;
    double m_msPerUpdate;  // 16.67 = 60fps, 33.33 = 30fps
  };

}


#endif // RUNNER_H

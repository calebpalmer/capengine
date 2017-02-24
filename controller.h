#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <memory>
#include <vector>
#include <SDL2/SDL.h>

#include "IEventSubscriber.h"

namespace CapEngine{

  class Controller{
    friend class ControllerManager;

  public:
    Controller(int joystickIndex);
    ~Controller();
    int getId() const;
    int getIndex() const;
    static std::vector<std::shared_ptr<Controller> > getConnectedControllers();
    std::string getName() const;
    SDL_GameController* getGameController() const;

  private:
    Controller(const Controller&);
    Controller& operator=(const Controller&);
    
    static std::vector<std::shared_ptr<Controller> > s_controllers;
    static bool s_controllersEnumerated;
    SDL_GameController* m_pController;
    int m_joystickId;
    int m_joystickIndex;

  };

  class ControllerManager : public IEventSubscriber {
  private:
    ControllerManager();
    ControllerManager(const ControllerManager&);
    ControllerManager& operator=(const ControllerManager&);

    static ControllerManager* s_controllerManager;

  public:
    static void initialize();
    void receiveEvent(SDL_Event event, Time* time);
    
  };
}

#endif // CONTROLLER_H


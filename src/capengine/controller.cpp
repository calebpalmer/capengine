#include "controller.h"

#include <sstream>

#include "CapEngineException.h"
#include "EventDispatcher.h"
#include "locator.h"
#include "logger.h"

using namespace std;
using namespace CapEngine;

bool Controller::s_controllersEnumerated = false;
vector<shared_ptr<Controller>> Controller::s_controllers;

Controller::Controller(int joystickIndex) : m_joystickIndex(joystickIndex)
{
  if (SDL_IsGameController(joystickIndex)) {
    m_pController = nullptr;

    SDL_Joystick *joystick = SDL_JoystickOpen(joystickIndex);
    m_joystickId = SDL_JoystickInstanceID(joystick);
    SDL_JoystickClose(joystick);

    m_pController = SDL_GameControllerOpen(joystickIndex);
    if (m_pController == nullptr) {
      ostringstream errorMsg;
      errorMsg << "Could not open gamecontroller " << joystickIndex << ": "
               << SDL_GetError();
      throw CapEngineException(errorMsg.str());
    }
  } else {
    ostringstream errorMsg;
    errorMsg << joystickIndex
             << " is not a valid index or it is not a GameController";
    throw CapEngineException(errorMsg.str());
  }
}

Controller::~Controller() { SDL_GameControllerClose(m_pController); }

vector<shared_ptr<Controller>> Controller::getConnectedControllers()
{
  // Make sure controllers only enumerated once because hereafter the event
  // system will be used to add/remove Controllers
  if (!s_controllersEnumerated) {
    for (int i = 0; i < SDL_NumJoysticks(); i++) {
      if (SDL_IsGameController(i)) {
        try {
          auto pController = std::make_shared<Controller>(i);
          s_controllers.push_back(pController);
        } catch (CapEngineException &e) {
          Locator::logger->log(e.what(), Logger::CWARNING, __FILE__, __LINE__);
        }
      }
    }
    s_controllersEnumerated = true;
  }

  return s_controllers;
}

string Controller::getName() const
{
  string name(SDL_GameControllerName(m_pController));
  return name;
}

int Controller::getId() const { return m_joystickId; }

int Controller::getIndex() const { return m_joystickIndex; }

SDL_GameController *Controller::getGameController() const
{
  return m_pController;
}

// ControllerManager members
ControllerManager *ControllerManager::s_controllerManager = nullptr;

void ControllerManager::initialize()
{
  if (s_controllerManager == nullptr) {
    s_controllerManager = new ControllerManager();
  }
}

ControllerManager::ControllerManager()
{
  IEventSubscriber::subscribe(Locator::eventDispatcher, controllerEvent);
}

void ControllerManager::receiveEvent(SDL_Event event, CapEngine::Time *time)
{
  // add new controller
  if (event.type == SDL_CONTROLLERDEVICEADDED) {
    int joystickIndex = ((SDL_ControllerDeviceEvent *)&event)->which;
    auto p_controller = std::make_shared<Controller>(joystickIndex);
    Controller::s_controllers.push_back(p_controller);

    ostringstream msg;
    msg << "Controller with id " << p_controller->getId() << " was found.";
    Locator::logger->log(msg.str(), Logger::CDEBUG, __FILE__, __LINE__);
  }

  // remove existing controller
  if (event.type == SDL_CONTROLLERDEVICEREMOVED) {
    int instanceId = ((SDL_ControllerDeviceEvent *)&event)->which;
    auto controllers = &(Controller::s_controllers);
    auto i = controllers->begin();
    for (; i < controllers->end(); i++) {
      if ((*i)->getId() == instanceId) {
        break;
      }
    }

    controllers->erase(i);
    ostringstream msg;
    msg << "Controller with id " << instanceId << " was found.";
    Locator::logger->log(msg.str(), Logger::CDEBUG);
  }
}

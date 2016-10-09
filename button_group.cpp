#include "button_group.h"

#include <assert.h>
#include <sstream>
#include <cmath>

#include "locator.h"
#include "logger.h"

using namespace CapEngine;
using namespace std;

ButtonGroup::ButtonGroup() : m_activeButtonIndex(0) {
  //Locator::eventDispatcher->subscribe(this, keyboardEvent | controllerEvent);
  IEventSubscriber::subscribe(Locator::eventDispatcher, keyboardEvent | controllerEvent);
}

ButtonGroup::~ButtonGroup(){
  //Locator::eventDispatcher->unsubscribe(this);
}

void ButtonGroup::update(){
  for(unsigned int i = 0; i < m_buttons.size(); i++){
    m_buttons[i]->update();
    if(i == m_activeButtonIndex){
      m_buttons[i]->setSelected(true);
    }
    else{
      m_buttons[i]->setSelected(false);      
    }
  }

}

void ButtonGroup::render(){
  for(auto& i : m_buttons){
    i->render();
  }
}

void ButtonGroup::receiveEvent(const SDL_Event event, CapEngine::Time* time){
  // keyboard
  if(event.type == SDL_KEYUP){
    const SDL_KeyboardEvent* keyboardEvent = reinterpret_cast<const SDL_KeyboardEvent*>(&event);
    if(keyboardEvent->keysym.sym == SDLK_UP){
      ostringstream msg;
      m_activeButtonIndex = static_cast<int>(std::abs(static_cast<double>(m_activeButtonIndex - 1))) % m_buttons.size();
      msg << "Button group index changed to " << m_activeButtonIndex;
      Locator::logger->log(msg.str(), Logger::CDEBUG);
    }
    if(keyboardEvent->keysym.sym == SDLK_DOWN){
      ostringstream msg;
      m_activeButtonIndex = (m_activeButtonIndex + 1) % m_buttons.size();
      msg << "Button group index changed to " << m_activeButtonIndex;
      Locator::logger->log(msg.str(), Logger::CDEBUG);
    }
    // if enter, activate current button
    if(keyboardEvent->keysym.sym == SDLK_RETURN){
      m_buttons[m_activeButtonIndex]->executeCallback();
    }
  }

  // game controller
  if(m_controllers.size() > 0 && event.type == SDL_CONTROLLERBUTTONUP){
    const SDL_ControllerButtonEvent* controllerEvent = reinterpret_cast<const SDL_ControllerButtonEvent*>(&event);
    SDL_GameControllerButton button = (SDL_GameControllerButton)controllerEvent->button;
    if(button == SDL_CONTROLLER_BUTTON_DPAD_UP){
      ostringstream msg;
      m_activeButtonIndex = static_cast<int>(std::abs(static_cast<double>(m_activeButtonIndex - 1))) % m_buttons.size();
      msg << "Button group index changed to " << m_activeButtonIndex;
      Locator::logger->log(msg.str(), Logger::CDEBUG);
    }
    else if(button == SDL_CONTROLLER_BUTTON_DPAD_DOWN){
      ostringstream msg;
      m_activeButtonIndex = (m_activeButtonIndex + 1) % m_buttons.size();
      msg << "Button group index changed to " << m_activeButtonIndex;
      Locator::logger->log(msg.str(), Logger::CDEBUG);
    }
    else if(button == SDL_CONTROLLER_BUTTON_A){
      m_buttons[m_activeButtonIndex]->executeCallback();
    }
  }

}

void ButtonGroup::addButton(std::unique_ptr<Button> pbutton){
  m_buttons.push_back(std::move(pbutton));
}

void ButtonGroup::removeButton(int index){
  
}

void ButtonGroup::listenController(shared_ptr<Controller> pController){
  m_controllers.push_back(pController);
}

void ButtonGroup::forgetController(shared_ptr<Controller> pController){
  // doesn't handle duplicates
  auto i = m_controllers.begin();
  for( ; i != m_controllers.end(); i++){
    if((*i)->getId() == pController->getId()){
      break;
    }
  }
  if(i != m_controllers.end()){
    m_controllers.erase(i);
  }
}

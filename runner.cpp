#include "runner.h"
#include "locator.h"
#include "filesystem.h"
#include "game_management.h"

#include <sstream>

using namespace CapEngine;

Runner::Runner() : m_quit(false), m_msPerUpdate(16.67), m_showFPS(false) {}

Runner* Runner::s_pRunner = nullptr;

Runner& Runner::getInstance(){
  if(s_pRunner == nullptr){
    s_pRunner = new Runner();
  }
  return *s_pRunner;
}

void Runner::popState(){
  if(m_gameStates.size() > 0){
    auto pPoppedState = std::move(m_gameStates.back());
    m_gameStates.pop_back();
    if(pPoppedState->onDestroy() == false){
      Locator::logger->log("Failed to destroy popped state", Logger::CWARNING);
    }
  }
}

void Runner::pushState(std::unique_ptr<GameState> pGameState){
  m_gameStates.push_back(std::move(pGameState));
  if(m_gameStates.back()->onLoad() == false){
    Locator::logger->log("Failed to init pushed state", Logger::CWARNING);
  }
}

void Runner::switchState(std::unique_ptr<GameState> pGameState){
  while(!m_gameStates.empty()){
    this->popState();
  }
  this->pushState(std::move(pGameState));
}

void Runner::end(){
  m_quit = true;
}

void Runner::loop() {
  int subscriptionMask = mouseEvent | keyboardEvent | systemEvent | windowEvent;
  Locator::eventDispatcher->subscribe(this, subscriptionMask);
  double previous = currentTime();
  double lag = 0.0;
  while(!m_quit){
    double current = currentTime();
    double elapsed = current - previous;
    previous = current;
    lag += elapsed;
    
    //process input
    Locator::eventDispatcher->getEvents();
    if(Locator::eventDispatcher->hasEvents()){
      Locator::eventDispatcher->flushQueue();
    }
    
    while(lag >= m_msPerUpdate){
      update();
      lag -= m_msPerUpdate;
    }
    render(1.0);
  }
  CapEngine::destroy();
}

void Runner::exit(){
  m_quit = true;
}

void Runner::update(){
  (m_gameStates.back())->update(m_msPerUpdate);
}

void Runner::render(double frameFactor){
  Locator::videoManager->clearScreen();
  m_gameStates.back()->render();
  Locator::videoManager->drawScreen();
}

void Runner::receiveEvent(const SDL_Event event, CapEngine::Time* time){
  if(event.type == SDL_WINDOWEVENT){
    if(event.window.event == SDL_WINDOWEVENT_CLOSE){
      m_quit = true;
    }
  }
  
  // exit when 'q' is pressed
  if ((event.type == SDL_KEYUP && ((SDL_KeyboardEvent*)&event)->keysym.sym == SDLK_q)
      || event.type == SDL_QUIT){
    m_quit = true;
    Locator::logger->log("quitting. ", Logger::CDEBUG);
    return;
  }
  // update the keyboard
  else if(event.type == SDL_KEYUP || event.type == SDL_KEYDOWN){
    SDL_Keycode ksym = ((SDL_KeyboardEvent*)&event)->keysym.sym;
    switch(ksym){
    case SDLK_UP:
      Locator::keyboard->keyMap[Keyboard::CAP_KEYUP].state = (event.type == SDL_KEYUP ? Keyboard::CAP_UNPRESSED : Keyboard::CAP_PRESSED);
      break;
    case SDLK_DOWN:
      Locator::keyboard->keyMap[Keyboard::CAP_KEYDOWN].state = (event.type == SDL_KEYUP ? Keyboard::CAP_UNPRESSED : Keyboard::CAP_PRESSED);      
      break;
    case SDLK_LEFT:
      Locator::keyboard->keyMap[Keyboard::CAP_KEYLEFT].state = (event.type == SDL_KEYUP ? Keyboard::CAP_UNPRESSED : Keyboard::CAP_PRESSED);
      break;
    case SDLK_RIGHT:
      Locator::keyboard->keyMap[Keyboard::CAP_KEYRIGHT].state = (event.type == SDL_KEYUP ? Keyboard::CAP_UNPRESSED : Keyboard::CAP_PRESSED);
      break;
    case SDLK_SPACE:
      Locator::keyboard->keyMap[Keyboard::CAP_SPACE].state = (event.type == SDL_KEYUP ? Keyboard::CAP_UNPRESSED : Keyboard::CAP_PRESSED);
      break;
    case SDLK_RETURN:
      Locator::keyboard->keyMap[Keyboard::CAP_ENTER].state = (event.type == SDL_KEYUP ? Keyboard::CAP_UNPRESSED : Keyboard::CAP_PRESSED);
      break;
    case SDLK_BACKSPACE:
      Locator::keyboard->keyMap[Keyboard::CAP_BACKSPACE].state = (event.type == SDL_KEYUP ? Keyboard::CAP_UNPRESSED : Keyboard::CAP_PRESSED);
      break;
    case SDLK_ESCAPE:
      Locator::keyboard->keyMap[Keyboard::CAP_ESCAPE].state = (event.type == SDL_KEYUP ? Keyboard::CAP_UNPRESSED : Keyboard::CAP_PRESSED);
      break;

    default:
      break;
    }
  }
  // update the mouse position 
  else if(event.type == SDL_MOUSEMOTION) {
    int x = ((SDL_MouseMotionEvent*)&event)->x;
    int y = ((SDL_MouseMotionEvent*)&event)->y;
    Locator::mouse->setx(x);
    Locator::mouse->sety(y);
  }
  // update the mouse buttons
  else if(event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP){
    if(((SDL_MouseButtonEvent*)&event)->type == SDL_MOUSEBUTTONDOWN){
      switch(((SDL_MouseButtonEvent*)&event)->button){
      case SDL_BUTTON_LEFT:
	Locator::mouse->setButtonState(0, true);
	break;
      case SDL_BUTTON_MIDDLE:
	Locator::mouse->setButtonState(1, true);
	break;
      case SDL_BUTTON_RIGHT:
	Locator::mouse->setButtonState(2, true);
	break;
      }
    }
    if(((SDL_MouseButtonEvent*)&event)->type == SDL_MOUSEBUTTONUP){
      switch(((SDL_MouseButtonEvent*)&event)->button){
      case SDL_BUTTON_LEFT:
	Locator::mouse->setButtonState(0, false);
	break;
      case SDL_BUTTON_MIDDLE:
	Locator::mouse->setButtonState(1, false);
	break;
      case SDL_BUTTON_RIGHT:
	Locator::mouse->setButtonState(2, false);
	break;
      }
    }
  }
  // check to see if fps should be shown
  if(event.type == SDL_KEYUP){
    SDL_Keycode ksym = ((SDL_KeyboardEvent*)&event)->keysym.sym;
    if(ksym == SDLK_TAB){
      if(m_showFPS == true){
	m_showFPS = false;
	Locator::videoManager->displayFPS(false);
      }
      else{
	m_showFPS = true;
	std::ostringstream ttfStream;
	ttfStream << getCurrentDir() << "/res/tahoma.ttf";
	Uint8 r = 255;
	Uint8 g = 255;
	Uint8 b = 255;
	Locator::videoManager->displayFPS(true, ttfStream.str(), r, g, b);
      }

    }
  }

}

#include "mouse.h"

#include "CapEngineException.h"

using namespace std;
using namespace CapEngine;

Mouse::Mouse(int numButtons){
  for(int i = 0; i < numButtons; i++){
    m_buttonStates.push_back(false);
  }
}

int Mouse::getx() const{
  return m_position.x;
}

int Mouse::gety() const{
  return m_position.y;
}

void Mouse::setx(int x) {
  m_position.x = x;
}

void Mouse::sety(int y){
  m_position.y = y;
}

bool Mouse::getButtonState(unsigned int buttonIndex) const{
  if(buttonIndex >= m_buttonStates.size()){
    throw CapEngineException("Mouse button does not exist");
  }

  return m_buttonStates[buttonIndex];
}

void Mouse::setButtonState(unsigned int buttonIndex,  bool state){
  if(buttonIndex >= m_buttonStates.size()){
    throw CapEngineException("Mouse button does not exist");
  }
  
  m_buttonStates[buttonIndex] = state;
}

#include "basicbutton.h"
#include "locator.h"
#include "asset_constants.h"

using namespace std;
using namespace CapEngine;

BasicButton::BasicButton(string text, int fontSize, Vector position, int width, int height) :
  m_text(text), m_width(width), m_height(height), m_selected(false), m_enabled(false), m_activated(true), m_position(position) {
   
  // create text surface
  FontManager fontManager;
  string font = "res/tahoma.ttf";
  m_pTextSurface = fontManager.getTextSurface(font, m_text, fontSize);

  // check dimensions of font surface to make sure they do not overflow the button size
  VideoManager* pVideoManager = Locator::videoManager;
  int fontWidth = pVideoManager->getSurfaceWidth(m_pTextSurface);
  int fontHeight = pVideoManager->getSurfaceHeight(m_pTextSurface);

  if(fontWidth > m_width || fontHeight > m_height){
    Locator::logger->log("font exceeds button size", Logger::CWARNING);
  }
}

BasicButton::~BasicButton(){
  Locator::videoManager->closeSurface(m_pTextSurface);
}

void BasicButton::update() {
  Mouse* mouse = Locator::mouse;
  // if the mouse button is pressed and it is on top of the button, update button pressed flag
  if(m_activated == false && mouse->getButtonState(0) == true){
    int x, y;
    x = mouse->getx();
    y = mouse->gety();
    Rectangle buttonRect(m_position.x, m_position.y, m_width, m_height);
    Relation relation = MBRRelate(x, y, buttonRect);
    if(relation == INSIDE){
      m_selected = true;
    }
  }
  // if mouse button is depressed and mouse is outside of the button, unset the flag
  if(m_activated == true && mouse->getButtonState(0) == false){
    int x, y;
    x = mouse->getx();
    y = mouse->gety();
    Rectangle buttonRect(m_position.x, m_position.y, m_width, m_height);
    Relation relation = MBRRelate(x, y, buttonRect);
    if(relation == INSIDE || relation == TOUCH){
      // perform some action
      // how?  register callback function?
      // set a flag and something can poll for it?
    }
    else{
      m_selected = false;
    }
  }
}

void BasicButton::render() {
  Rectangle destRect(m_position.x, m_position.y, m_width, m_height);
  // if flag is set,
    // // draw frame 1
  if(m_activated){
    Locator::assetManager->draw(BASICBUTTONTEXTUREID, destRect, 0, 0);
    }
  // // draw text on top
  // if flag is not set
  else{
    Locator::assetManager->draw(BASICBUTTONTEXTUREID, destRect, 0, 1);
  }
  // // draw frame 0
}

void BasicButton::setEnabled(bool enabled) {
  m_enabled = enabled;
}

bool BasicButton::isEnabled() const {
  return m_enabled;
}

int BasicButton::getWidth() const {
  return m_width;
}

int BasicButton::getHeight() const {
  return m_height;
}

void BasicButton::setWidth(int width) {
  m_width = width;
}

void BasicButton::setHeight(int height) {
  m_height = height;
}

string BasicButton::getText() const {
  return m_text;
}

void BasicButton::setText(const string text) {
  m_text = text;
}



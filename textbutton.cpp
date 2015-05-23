#include "textbutton.h"

#include "locator.h"

using namespace std;
using namespace CapEngine;

TextButton::TextButton(string text, string font, int fontSize, Vector position)
  : m_text(text), m_font(font), m_fontSize(fontSize), m_selected(false),  m_position(position)
{
  // get surface
  FontManager fontManager;
  m_pTextSurfaceInactive = fontManager.getTextSurface(m_font, m_text, m_fontSize, 0xFF, 0xFF, 0xFF);
  m_pTextSurfaceActive = fontManager.getTextSurface(m_font, m_text, m_fontSize, 0xBA, 0xBA, 0xBA);
  // set width and height

  m_width = Locator::videoManager->getSurfaceWidth(m_pTextSurfaceInactive);
  m_height = Locator::videoManager->getSurfaceHeight(m_pTextSurfaceInactive);

  Locator::eventDispatcher->subscribe(this, mouseEvent);
}

TextButton::~TextButton(){
  // free surfaces
  Locator::videoManager->closeSurface(m_pTextSurfaceActive);
  Locator::videoManager->closeSurface(m_pTextSurfaceInactive);
  Locator::eventDispatcher->unsubscribe(this);
}

void TextButton::receiveEvent(const SDL_Event* event, CapEngine::Time* time){
  if(event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEBUTTONUP){
    if(mouseInButton(Vector(Locator::mouse->getx(), Locator::mouse->gety()))){
      if(event->type == SDL_MOUSEBUTTONDOWN){
	m_selected = true;
      }
      else{  // MOUSEBUTTONUP
	if(m_callback != nullptr){
	  m_selected = false;
	  m_callback(m_context);
	}
      }
    }
    else{
      m_selected = false;
    }
  }
  delete event;
}


void TextButton::update() {
  if(!(mouseInButton(Vector(Locator::mouse->getx(), Locator::mouse->gety())))){
    m_selected = false;
  }
}

void TextButton::render() {
  Rect destRect;
  destRect.x = m_position.x;
  destRect.y = m_position.y;
  Rect srcRect;
  srcRect.x = 0.0;
  srcRect.y = 0.0;
  srcRect.w = m_width;
  srcRect.h = m_height;
  // if flag is set,
  if(m_selected){
    Locator::videoManager->drawSurface(m_pTextSurfaceActive, &srcRect, &destRect);
    }
  // if flag is not set
  else{
    Locator::videoManager->drawSurface(m_pTextSurfaceInactive, &srcRect, &destRect);
  }
}

void TextButton::setEnabled(bool enabled) {
  m_enabled = enabled;
}

bool TextButton::isEnabled() const {
  return m_enabled;
}

int TextButton::getWidth() const {
  return m_width;
}

int TextButton::getHeight() const {
  return m_height;
}

void TextButton::setWidth(int width) {
  m_width = width;
}

void TextButton::setHeight(int height) {
  m_height = height;
}

string TextButton::getText() const {
  return m_text;
}

void TextButton::setText(const string text) {
  m_text = text;
}

void TextButton::setPosition(const Vector position){
  m_position = position;
}

void TextButton::registerCallback(void (*callback)(void *), void* context) {
  m_callback = callback;
  m_context = context;
}

bool TextButton::mouseInButton(Vector position){
    Rectangle buttonRect(m_position.x, m_position.y, m_width, m_height);
    Relation relation = MBRRelate(position.x, position.y, buttonRect);
    if(relation == INSIDE || relation == TOUCH){
      return true;
    }
    else{
      return false;
    }
}




#include "keyboard.h"
#include "CapEngineException.h"

#include <SDL2/SDL.h>
#include <iostream>

namespace
{

SDL_Scancode KeyToSDLScancode(CapEngine::Keyboard::Key in_key)
{
  using namespace CapEngine;

  switch (in_key) {
  case Keyboard::CAP_KEYUP:
    return SDL_SCANCODE_UP;
  case Keyboard::CAP_KEYDOWN:
    return SDL_SCANCODE_DOWN;
  case Keyboard::CAP_KEYLEFT:
    return SDL_SCANCODE_LEFT;
  case Keyboard::CAP_KEYRIGHT:
    return SDL_SCANCODE_RIGHT;
  case Keyboard::CAP_SPACE:
    return SDL_SCANCODE_SPACE;
  case Keyboard::CAP_ENTER:
    return SDL_SCANCODE_RETURN;
  case Keyboard::CAP_BACKSPACE:
    return SDL_SCANCODE_BACKSPACE;
  case Keyboard::CAP_ESCAPE:
    return SDL_SCANCODE_ESCAPE;
  case Keyboard::CAP_Z:
    return SDL_SCANCODE_Z;
  case Keyboard::CAP_Y:
    return SDL_SCANCODE_Y;
  case Keyboard::CAP_LCTRL:
    return SDL_SCANCODE_LCTRL;
  default:
    CAP_THROW(CapEngineException("Unknown Key"));
  }
}
} // namespace

namespace CapEngine
{

Keyboard::Keyboard()
    : m_keyMap({
          {CAP_KEYUP, {CAP_UNPRESSED}},
          {CAP_KEYDOWN, {CAP_UNPRESSED}},
          {CAP_KEYLEFT, {CAP_UNPRESSED}},
          {CAP_KEYRIGHT, {CAP_UNPRESSED}},
          {CAP_SPACE, {CAP_UNPRESSED}},
          {CAP_ENTER, {CAP_UNPRESSED}},
          {CAP_BACKSPACE, {CAP_UNPRESSED}},
          {CAP_ESCAPE, {CAP_UNPRESSED}},
          {CAP_Z, {CAP_UNPRESSED}},
          {CAP_Y, {CAP_UNPRESSED}},
          {CAP_LCTRL, {CAP_UNPRESSED}},
      })
{
}

/**
 * @brief Update the keyboard map.
 */
void Keyboard::update()
{
  const Uint8 *state = SDL_GetKeyboardState(NULL);
  for (auto &&[key, value] : m_keyMap) {
    SDL_Scancode scancode = KeyToSDLScancode(key);
    if (state[scancode]) {
      m_keyMap[key] = KeyInfo{CAP_PRESSED};
    } else
      m_keyMap[key] = KeyInfo{CAP_UNPRESSED};
  }
}

/**
 * @brief Return the state of a Key.
 * @param[in] in_key The key.
 * @return The KeyInfo.
 */
Keyboard::KeyInfo Keyboard::key(Keyboard::Key in_key) const
{
  const auto keyInfo = m_keyMap.find(in_key);
  CAP_THROW_ASSERT(keyInfo != m_keyMap.end(),
                   "Key not found in CapEngine::Keyboard");
  return keyInfo->second;
}
} // namespace CapEngine

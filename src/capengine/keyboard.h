#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <map>

namespace CapEngine
{
class Keyboard
{
public:
  enum Key {
      CAP_KEYUP,
      CAP_KEYDOWN,
      CAP_KEYLEFT,
      CAP_KEYRIGHT,
      CAP_SPACE,
      CAP_ENTER,
      CAP_BACKSPACE,
      CAP_ESCAPE,
      CAP_Z,
      CAP_Y,
      CAP_W,
      CAP_A,
      CAP_S,
      CAP_D,
      CAP_LCTRL
  };

  enum KeyState { CAP_UNPRESSED, CAP_PRESSED };

  struct KeyInfo {
      KeyState state;
  };

  Keyboard();
  void update();
  KeyInfo key(Key in_key) const;

private:
  std::map<Key, KeyInfo> m_keyMap;
};
} // namespace CapEngine

#endif

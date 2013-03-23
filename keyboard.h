#fndef KEYBOARD_H
#define KEYBOARD_H

#include <map>

namespace CapEngine{
  class Keyboard{
  public:
    enum Keys {
      CAP_KEYUP,
      CAP_KEYDOWN,
      CAP_KEYLEFT,
      CAP_KEYRIGHT
    };

    enum KeyState {
      CAP_UNPRESSED,
      CAP_PRESSED
    };
    
    struct KeyInfo{
      KeyState state;
    };

    std::map<Keys, KeyInfo> keyMap;
  }
}

#endif

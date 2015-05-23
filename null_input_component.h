#ifndef NULL_INPUT_COMPONENT_H
#define NULL_INPUT_COMPONENT_H

#include "gameobject.h"

namespace CapEngine{
  
  class NullInputComponent : public InputComponent {

    virtual void update(GameObject* object) {}

  };

}
#endif // NULL_INPUT_COMPONENT_H

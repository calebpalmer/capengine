#ifndef NULL_AI_COMPONENT
#define NULL_AI_COMPONENT

#include "gameobject.h"

#include <memory>

namespace CapEngine {

  class NullAIComponent :  public AIComponent {
  public:
    virtual void update(GameObject* object) {}
    virtual AIComponent* clone() const {
      std::unique_ptr<AIComponent> newAIComponent(new NullAIComponent);
      return newAIComponent.release();
    }
  };

}
#endif

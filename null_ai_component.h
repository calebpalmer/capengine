#ifndef NULL_AI_COMPONENT
#define NULL_AI_COMPONENT

#include "gameobject.h"

#include <memory>

namespace CapEngine {

  class NullAIComponent :  public AIComponent {
  public:
    void update(GameObject* /*object*/) override {}
    std::unique_ptr<AIComponent> clone() const override {
      std::unique_ptr<AIComponent> newAIComponent(new NullAIComponent);
      return newAIComponent;
    }
  };

}
#endif

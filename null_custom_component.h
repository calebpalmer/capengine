#ifndef NULL_CUSTOM_COMPONENT
#define NULL_CUSTOM_COMPONENT

#include "gameobject.h"

#include <memory>

namespace CapEngine
{

//! A Custom component that does nothing.
class NullCustomComponent : public CustomComponent
{
public:
  void update(GameObject &object, double ms) override {}

  std::unique_ptr<Component> clone() const override
  {
    return std::make_unique<NullCustomComponent>();
  }
};

} // namespace CapEngine
#endif

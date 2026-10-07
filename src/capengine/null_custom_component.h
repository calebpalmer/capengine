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

};

} // namespace CapEngine
#endif

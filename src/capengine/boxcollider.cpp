#include "boxcollider.h"

#include "componentfactory.h"
#include "componentutils.h"
#include "gameobject.h"
#include "scene2dschema.h"
#include "scene2dutils.h"

namespace CapEngine
{

//! Constructor
/**
 \param in_rectangle
   The rectangle.
*/
BoxCollider::BoxCollider(Rectangle in_rectangle, Anchor in_anchor) : m_box(in_rectangle), m_anchor(in_anchor)
{
}

//! Makes a BoxCollider from json.
/**
 \param in_json
   The jsn
 \return
   The constructed BoxCollider
*/
std::unique_ptr<BoxCollider> BoxCollider::makeComponent(const jsoncons::json& in_json)
{
    try {
        Rectangle rect = JSONUtils::readRectangle(in_json[Schema::Components::kBox]);
        // TODO read in an anchor position
        return std::make_unique<BoxCollider>(std::move(rect));
    }
    catch (jsoncons::json_exception& e) {
        throw ComponentCreationException(ComponentUtils::componentTypeToString(ComponentType::Physics), kType, in_json,
                                         e.what());
    }
}

//! Registers this component with the Component Factory
/**
 \param in_factor
   The component factory.
*/
void BoxCollider::registerConstructor(ComponentFactory &in_factory)
{
  in_factory.registerComponentType(
      ComponentUtils::componentTypeToString(ComponentType::Physics), kType,
      makeComponent);
}

void BoxCollider::update(GameObject &object, double timestep)
{
  Vector const &position = object.getPosition();

  m_box.x = position.getX();
  m_box.y = position.getY();
}

} // namespace CapEngine

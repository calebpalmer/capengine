#ifndef CAPENGINE_BOXCOLLIDERCOMPONENT_H
#define CAPENGINE_BOXCOLLIDERCOMPONENT_H

#include "collision.h"
#include "components.h"
#include "gameobject.h"

#include <jsoncons/json.hpp>

namespace CapEngine {

// forward declaration
class ComponentFactory;

class BoxCollider : public PhysicsComponent {
   public:
    BoxCollider(Rectangle in_rectangle, Anchor in_anchor = Anchor::Center);
    ~BoxCollider() override = default;

    std::unique_ptr<Component> clone() const override;

    static std::unique_ptr<BoxCollider> makeComponent(const jsoncons::json& in_json);
    static void registerConstructor(ComponentFactory& in_factory);

    void update(GameObject& object, double timestep) override;
    std::optional<Rectangle> boundingPolygon(const GameObject& object) const override;

   public:
    static inline constexpr char kType[] = "BoxCollider";

   private:
    Rectangle m_box;  //!< The box.
    Anchor m_anchor;  //!< The anchor for the box relative to position
};

//! \copydoc PhysicsComponent::boundingPolygon
inline std::optional<Rectangle> BoxCollider::boundingPolygon(const GameObject& object) const
{
    // return m_box;
    const auto position = object.getPosition();

    // assume bottom left anchor
    Rectangle rect{position.getX(), position.getY(), m_box.width, m_box.height};
    // check if center anchor
    if (m_anchor == Anchor::Center) {
        rect.x = position.getX() - (m_box.width / 2.0);
        rect.y = position.getY() - (m_box.height / 2.0);
    }
    return rect;
}

// \copydoc Component::clone
inline std::unique_ptr<Component> BoxCollider::clone() const
{
    return std::make_unique<BoxCollider>(*this);
}

}  // namespace CapEngine

#endif // CAPENGINE_BOXCOLLIDERCOMPONENT_H

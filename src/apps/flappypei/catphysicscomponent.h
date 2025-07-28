#ifndef CATPHYSICSCOMPONENT_H
#define CATPHYSICSCOMPONENT_H

#include <capengine/components.h>
#include <capengine/gameevent.h>

namespace FlappyPei {

class CatPhysicsComponent : public CapEngine::PhysicsComponent {
   public:
    CatPhysicsComponent(int in_gapLocation, int in_gapSize);
    ~CatPhysicsComponent() override = default;

    CatPhysicsComponent(CatPhysicsComponent const&) = default;
    CatPhysicsComponent(CatPhysicsComponent&&) = default;
    CatPhysicsComponent& operator=(CatPhysicsComponent const&) = default;
    CatPhysicsComponent& operator=(CatPhysicsComponent&&) = default;

    void update(CapEngine::GameObject& object, double timestep) override;
    [[nodiscard]] std::optional<CapEngine::Rectangle> boundingPolygon(
        const CapEngine::GameObject& object) const override;
    [[nodiscard]] std::unique_ptr<CapEngine::Component> clone() const override;
    [[nodiscard]] CapEngine::CollisionType collides([[maybe_unused]] CapEngine::Rectangle const& in_mbr) const override;

    [[nodiscard]] int gapLocation() const;
    [[nodiscard]] int gapSize() const;

   private:
    int m_gapLocation = 0;
    int m_gapSize = 0;
    CapEngine::Rectangle m_topMbr;
    CapEngine::Rectangle m_bottomMbr;
};
}  // namespace FlappyPei

#endif  // CATPHYSICSCOMPONENT_H

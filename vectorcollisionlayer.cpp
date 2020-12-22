#include "vectorcollisionlayer.h"
#include "collision.h"

namespace CapEngine
{

VectorCollisionLayer::VectorCollisionLayer(const jsoncons::json &in_json) {}

void registerConstructor(LayerFactory &in_factory)
{
  in_factory.registerLayerType(
      "test", [](const jsoncons::json &in_json) { return nullptr; });
}

void render(const Camera2d &in_camera, uint32_t in_windowId) {}

Layer::CollisionType_t
    VectorCollisionLayer::checkCollisions(const GameObject &in_object) const
{
  return {};
}

std::optional<GameObject>
    VectorCollisionLayer::resolveCollisions(const GameObject &in_object) const
{
  return std::nullopt;
}

} // namespace CapEngine

#ifndef CAPENGINE_VECTORCOLLISIONLAYER_H
#define CAPENGINE_VECTORCOLLISIONLAYER_H

#include "layer.h"
#include "layerfactory.h"

#include "jsoncons/json.hpp"

namespace CapEngine
{

//! Layer that implements collisions using vector data
class VectorCollisionLayer final : public Layer
{
  public:
    VectorCollisionLayer(const jsoncons::json &in_json);
    ~VectorCollisionLayer() override;

    static void registerConstructor(LayerFactory &in_factory);

    const std::string type() const override;
    void update(double in_ms) override {}
    void render(const Camera2d &in_camera, uint32_t in_windowId) override{};
    bool canCollide() const override;
    CollisionType_t checkCollisions(const GameObject &in_object) const override;
    bool resolveCollisions(const GameObject &in_object) const override;
};

//! \override Layer::type()
inline const std::string VectorCollisionLayer::type() const
{
    return Schema::Scene2d::kBitmapCollisionLayer;
}

//! \copydoc Layer::canCollide
inline bool VectorCollisionLayer::canCollide() const { return true; }

} // namespace CapEngine

#endif /* CAPENGINE_VECTORCOLLISIONLAYER_H */

#ifndef CAPENGINE_BITMAPCOLLISIONLAYER_H
#define CAPENGINE_BITMAPCOLLISIONLAYER_H

#include "asset_manager.h"
#include "imagelayer.h"
#include "scene2dschema.h"
#include <boost/variant/variant.hpp>

namespace CapEngine
{

//! layer class for providing bitmap collisions.
class BitmapCollisionLayer final : public ImageLayer
{
public:
  BitmapCollisionLayer(int in_assetId, Rectangle in_position);
  ~BitmapCollisionLayer() override = default;

  static void registerConstructor(LayerFactory &in_factory);

  const std::string type() const override;
  void update(double in_ms) override {}
  void render(const Camera2d &in_camera, uint32_t in_windowId) override{};
  bool canCollide() const override;
  CollisionType_t checkCollisions(const GameObject &in_object) const override;
  std::optional<GameObject>
      resolveCollisions(const GameObject &in_object) const override;

private:
  std::vector<std::pair<CollisionType, Vector>>
      getCollisions(const GameObject &in_object) const;

  mutable std::optional<SoftwareImage> m_softwareImage;
};

//! \override Layer::type()
inline const std::string BitmapCollisionLayer::type() const
{
  return Schema::Scene2d::kBitmapCollisionLayer;
}

//! \copydoc Layer::canCollide
inline bool BitmapCollisionLayer::canCollide() const { return true; }

} // namespace CapEngine

#endif // CAPENGINE_BITMAPCOLLISIONLAYER_H

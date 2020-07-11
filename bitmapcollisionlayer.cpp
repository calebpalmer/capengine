#include "bitmapcollisionlayer.h"

#include "asset_manager.h"
#include "collision.h"
#include "gameobject.h"
#include "locator.h"

namespace CapEngine
{

//! Constructor.
/**
 \param in_assetID
   The asset id of the image to use.
 \param in_position
   The position of the layer.
*/
BitmapCollisionLayer::BitmapCollisionLayer(int in_assetId,
                                           Rectangle in_position)
    : ImageLayer(in_assetId, in_position)
{
}

//! \copydoc Layer::checkCollision
BitmapCollisionLayer::CollisionType
    BitmapCollisionLayer::checkCollision(const GameObject &in_object)
{
  assert(Locator::assetManager != nullptr);

  SoftwareImage softwareImage =
      Locator::assetManager->getSoftwareImage(m_assetId);
  assert(softwareImage.surface != nullptr);

  Vector temp;
  CapEngine::CollisionType collisionType = detectBitmapCollision(
      in_object.boundingPolygon(), softwareImage.surface, temp);
  if (collisionType != COLLISION_NONE) {
    return collisionType;
  }

  else {
    return std::nullopt;
  }
}

//! Register the layer constructor with a factory.
/**
 \param in_factory
   The factory to register with.
*/
void BitmapCollisionLayer::registerConstructor(LayerFactory &in_factory)
{
  in_factory.registerLayerType(Schema::Scene2d::kBitmapCollisionLayer,
                               [](const jsoncons::json &in_json) {
                                 return detail::makeImageLayer<ImageLayer>(
                                     in_json);
                               });
}

} // namespace CapEngine

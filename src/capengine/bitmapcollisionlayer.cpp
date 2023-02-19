#include "bitmapcollisionlayer.h"

#include "VideoManager.h"
#include "asset_manager.h"
#include "camera2d.h"
#include "collision.h"
#include "gameobject.h"
#include "locator.h"
#include "logger.h"

#include <optional>
#include <utility>

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
BitmapCollisionLayer::CollisionType_t
	BitmapCollisionLayer::checkCollisions(const GameObject &in_object) const
{
  return this->getCollisions(in_object);
}

std::vector<std::pair<CollisionType, Vector>>
	BitmapCollisionLayer::getCollisions(const GameObject &in_object) const
{
  assert(Locator::assetManager != nullptr);

  if (!m_softwareImage) {
	m_softwareImage = Locator::assetManager->getSoftwareImage(m_assetId);
  }
  assert(m_softwareImage->surface != nullptr);

  // std::cout << "Object position: " << in_object.getPosition() << std::endl;
  Rectangle mbr = in_object.boundingPolygon();
  // std::cout << "Object bounding retangle before y flip: " << mbr << std::endl;
  if (in_object.getYAxisOrientation() == YAxisOrientation::BottomZero) {
	mbr.y =
		Locator::videoManager->toScreenCoord(m_softwareImage->surface, mbr.y) -
		mbr.height;
  }

  // std::cout << "Object bounding rectangle: " << mbr << std::endl;

  return detectBitmapCollision(mbr, m_softwareImage->surface);
}

//! Register the layer constructor with a factory.
/**
 \param in_factory
   The factory to register with.
*/
void BitmapCollisionLayer::registerConstructor(LayerFactory &in_factory)
{
  in_factory.registerLayerType(
	  Schema::Scene2d::kBitmapCollisionLayer,
	  [](const jsoncons::json &in_json) {
		return detail::makeImageLayer<BitmapCollisionLayer>(in_json);
	  });
}

bool BitmapCollisionLayer::resolveCollisions(GameObject &in_object) const
{
    const int maxAttempts = 10;

    int numAttempts = 0;
    GameObject resolved = in_object;

    auto collisions = this->checkCollisions(in_object);
    while (numAttempts < maxAttempts) {
        if (collisions.size() == 0)
            return true;

        for (auto &&collision : collisions) {
            // tell the object about the collision and see if it can handle it
            if (in_object.handleCollision(collision.first, COLLISION_BITMAP,
                                          nullptr, collision.second))
                return true;

            const auto collisionType = collision.first;

            auto position = in_object.getPosition();

            if (collisionType == COLLISION_NONE)
                continue;

            else if (collisionType == COLLISION_LEFT) {
                position.setX(position.getX() + 1);
            }

            else if (collisionType == COLLISION_RIGHT) {
                position.setX(position.getX() - 1);
            }

            else if (collisionType == COLLISION_TOP) {
                position.setY(position.getY() - 1);
            }

            else if (collisionType == COLLISION_BOTTOM) {
                position.setY(position.getY() + 1);
            }

            in_object.setPosition(position);
        }

        numAttempts++;
        collisions = this->checkCollisions(in_object);
    }

    CAP_LOG(Locator::logger,
            "max attempts reached attempting to resolve collision",
            Logger::CERROR);
    return false;
}

} // namespace CapEngine

#ifndef CAPENGINE_BITMAPCOLLISIONLAYER_H
#define CAPENGINE_BITMAPCOLLISIONLAYER_H

#include "imagelayer.h"

namespace CapEngine {

//! layer class for providing bitmap collisions.
class BitmapCollisionLayer : public ImageLayer {
public:
	BitmapCollisionLayer(int in_assetId, Rectangle in_position);
	~BitmapCollisionLayer() override = default;

	static void registerConstructor(LayerFactory &in_factory);

	void update(double in_ms) override {}
	void render(const Camera2d &in_camera, uint32_t in_windowId) override {};
	bool canCollide() const override;
	CollisionType checkCollision(const GameObject &in_object) override;
};

//! \copydoc Layer::canCollide
inline bool BitmapCollisionLayer::canCollide() const{
	return true;
}

} // namespace CapEngine

#endif // CAPENGINE_BITMAPCOLLISIONLAYER_H

#include "imagelayer.h"

#include "locator.h"
#include "asset_manager.h"
#include "camera2d.h"

namespace CapEngine {

//! Constructor.
/** 
 \param in_assetID
   The id of the image asset.
*/
ImageLayer::ImageLayer(int in_assetId, Rectangle in_position)
	: m_assetId(in_assetId), m_position(std::move(in_position))
{
	assert(Locator::assetManager != nullptr);
	assert(Locator::assetManager->imageExists(in_assetId));
}

//! \copydoc Layer::render
void ImageLayer::render(const Camera2d &in_camera, uint32_t in_windowId){
	assert(Locator::videoManager != nullptr);
	assert(Locator::videoManager->isValidWindowId(in_windowId));
	assert(Locator::assetManager != nullptr);

	const Rectangle &viewport = in_camera.getViewingRectangle();
	
	// is the image in the camera view?
	const Relation relation = MBRRelate(m_position, viewport);
	if(relation == INSIDE || relation == TOUCH){

		// translate the position according to the position of the camera
		const Rectangle translatedPosition = toScreenCoords(in_camera, m_position, in_windowId, true);
		// let SDL crop what isn't visible
		Locator::assetManager->draw(in_windowId, m_assetId, translatedPosition);
	}

}

} // namespace CapEngine

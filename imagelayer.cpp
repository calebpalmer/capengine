#include "imagelayer.h"

#include "locator.h"
#include "asset_manager.h"

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
	
}

} // namespace CapEngine

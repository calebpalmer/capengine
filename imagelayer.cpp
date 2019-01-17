#include "imagelayer.h"

#include "locator.h"
#include "asset_manager.h"
#include "camera2d.h"
#include "layerfactory.h"
#include "scene2dschema.h"
#include "scene2dutils.h"

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
		const Rectangle translatedPosition = toScreenCoords(in_camera, m_position, in_windowId, false);
		// let SDL crop what isn't visible
		Locator::assetManager->draw(in_windowId, m_assetId, translatedPosition);
	}
}

//! Static function for creating image layers
/** 
 \param in_json
   The json to creat the layer from.
 \return 
   The layer.
*/
std::unique_ptr<ImageLayer> ImageLayer::createImageLayer(const jsoncons::json &in_json){
	try{
		const int assetId = in_json[Schema::Scene2d::kAssetId].as<int>();
		const Rectangle position = Utils::readRectangle(in_json[Schema::Scene2d::kPosition]);
		return std::make_unique<ImageLayer>(assetId, std::move(position));
	}

	catch(const std::exception &e){
		throw LayerCreationError(Schema::Scene2d::kImageLayerType, boost::diagnostic_information(e));
	}
}
	
//! Register the layer constructor with a factory.
/** 
 \param in_factory
   The factory to register with.
*/
void ImageLayer::registerConstructor(LayerFactory &in_factory) {
	LayerFactory& layerFactory = LayerFactory::getInstance();
	layerFactory.registerLayerType(Schema::Scene2d::kImageLayerType, createImageLayer);
}

} // namespace CapEngine

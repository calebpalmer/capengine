#include "locator.h"

#include "scene2dschema.h"
#include "layerfactory.h"
#include "gameobject.h"
#include "gameobjectutils.h"
#include "vector.h"

namespace CapEngine {

namespace {

//! updates the camera size based on the window size
/** 
 \param in_windowId
   The id of the window.
 \param io_camera
   The camera to update
*/
void updateCameraSize(uint32_t in_windowId, Camera2d &io_camera){
	assert(Locator::videoManager != nullptr);
	assert(Locator::videoManager->isValidWindowId(in_windowId));

	int width = 0;
	int height = 0;
	// TODO maybe this should be the scene size prior to scaling to fit window
	std::tie(width, height) = Locator::videoManager->getWindowLogicalResolution(in_windowId);
	io_camera.setWidth(width);
	io_camera.setHeight(height);
}

}

//! Constructor
/** 
 \param in_json
   The json describing the scene.
*/
template <class ObjectManager>
Scene2d<ObjectManager>::Scene2d(const jsoncons::json &in_json)
	: m_camera(0, 0)
{
	this->load(in_json);
}

//! Constructor
/** 
 \param in_json
   The json describing the scene.
 \param in_objectManager
   The object manager.
*/
template <class ObjectManager>
Scene2d<ObjectManager>::Scene2d(const jsoncons::json &in_json, ObjectManager in_objectManager)
	: Scene2d<ObjectManager>(in_json)
{
	m_objectManager = std::move(in_objectManager);
}

//! load the scene from json.
/** 
 \param in_json
   The json representation of the scene.
*/
template <class ObjectManager>
void Scene2d<ObjectManager>::load(const jsoncons::json &in_json) {
	try{
		using namespace Schema::Scene2d;
	
		// get the width and height of the scene
		m_sceneSize.width = in_json[kWidth].as<int>();
		m_sceneSize.height = in_json[kHeight].as<int>();

		// get the layers
		LayerFactory &layerFactory = LayerFactory::getInstance();
		for(auto && layer : in_json[kLayers].array_range()){
			std::unique_ptr<Layer> pLayer = layerFactory.makeLayer(layer);
			const int layerOrder = layer[kLayerOrder].as<int>();

			m_layers.emplace(layerOrder, std::move(pLayer));
		}

		// get the objects
		for(auto && objectJson : in_json[std::string(kObjects)].array_range()){
			try {
				GameObject object = makeObject(objectJson);
				auto pHeapObject = std::make_unique<GameObject>(object);
				m_objectManager.addObject(std::move(pHeapObject));
			}
			catch(const ObjectCreationError &e){
				// log and move on
				CAP_LOG_EXCEPTION(Locator::logger, e, Logger::CWARNING);
			}
		}
	}
	
	catch(const jsoncons::parse_error &e) {
		throw SceneLoadException(in_json, e.what());
	}
}

//! update function for the scene.
/** 
 \param in_ms
   The timestep.
*/
template <class ObjectManager>
void Scene2d<ObjectManager>::update(double in_ms) {
	for(auto && i : m_layers){
		assert(i.second != nullptr);
		i.second->update(in_ms);
	}
}

//! render function for the scene.
/** 
 \param  in_windowId
   The id of the window to render to.
*/
template <class ObjectManager>
void Scene2d<ObjectManager>::render(uint32_t in_windowId) {
	updateCameraSize(in_windowId, m_camera);

	// iterate in reverse order to draw layers in the proper order.
	for(auto i = m_layers.rbegin(); i != m_layers.rend(); ++i){
		assert(i->second != nullptr);
		i->second->render(m_camera, in_windowId);
	}

	// render objects
	for(auto && pObject : m_objectManager.getObjects(m_camera.getViewingRectangle())){
		assert(pObject != nullptr);
		pObject->render(m_camera, in_windowId);
	}
}

}

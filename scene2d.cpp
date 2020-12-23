#include "scene2d.h"
#include "CapEngineException.h"
#include "objectmanager.h"
#include "simpleobjectmanager.h"
#include <memory>

// implementation
namespace CapEngine
{

namespace
{

//! updates the camera size based on the window size
/**
 \param in_windowId
   The id of the window.
 \param io_camera
   The camera to update
*/
void updateCameraSize(uint32_t in_windowId, Camera2d &io_camera)
{
  assert(Locator::videoManager != nullptr);
  assert(Locator::videoManager->isValidWindowId(in_windowId));

  int width = 0;
  int height = 0;
  // TODO maybe this should be the scene size prior to scaling to fit window
  std::tie(width, height) =
      Locator::videoManager->getWindowLogicalResolution(in_windowId);
  io_camera.setWidth(width);
  io_camera.setHeight(height);
}
} // namespace

//! Constructor
/**
 \param in_json
   The json describing the scene.
*/
Scene2d::Scene2d(const jsoncons::json &in_json)
    : m_camera(0, 0), m_pObjectManager(std::make_shared<SimpleObjectManager>())
{
  this->load(in_json);
  Locator::insert(ObjectManager::kObjectManagerLocatorId, m_pObjectManager);
}

//! load the scene from json.
/**
 \param in_json
   The json representation of the scene.
*/
void Scene2d::load(const jsoncons::json &in_json)
{
  try {
    using namespace Schema::Scene2d;

    // get the width and height of the scene
    m_sceneSize.width = in_json[kWidth].as<int>();
    m_sceneSize.height = in_json[kHeight].as<int>();

    // get the layers
    LayerFactory &layerFactory = LayerFactory::getInstance();
    for (auto &&layer : in_json[kLayers].array_range()) {
      std::unique_ptr<Layer> pLayer = layerFactory.makeLayer(layer);
      const int layerOrder = layer[kLayerOrder].as<int>();

      std::cout << "Adding layer " << pLayer->type() << std::endl;
      m_layers.emplace(layerOrder, std::move(pLayer));
    }

    // get the objects
    for (auto &&objectJson : in_json[std::string(kObjects)].array_range()) {
      try {
        GameObject object = makeObject(objectJson);
        auto pHeapObject = std::make_unique<GameObject>(object);

        CAP_THROW_NULL(m_pObjectManager, "ObjectManager is null");
        m_pObjectManager->addObject(std::move(pHeapObject));
      } catch (const ObjectCreationError &e) {
        // log and move on
        CAP_LOG_EXCEPTION(Locator::logger, e, Logger::CWARNING);
      }
    }
  }

  catch (const jsoncons::parse_error &e) {
    throw SceneLoadException(in_json, e.what());
  }
}

//! update function for the scene.
/**
 \param in_ms
   The timestep.
*/
void Scene2d::update(double in_ms)
{
  // update layers
  for (auto &&i : m_layers) {
    assert(i.second != nullptr);
    i.second->update(in_ms);
  }

  // update objects
  CAP_THROW_NULL(m_pObjectManager, "ObjectManager is null");
  for (auto &&i : m_pObjectManager->getObjects()) {
    auto pUpdateObject = i->update(in_ms);
    if (!pUpdateObject) {
      CAP_LOG(Locator::logger, "GameObject::update returned nullptr",
              Logger::CWARNING);
      continue;
    }

    // collision detection
    // collision with layers?
    for (auto &&layer : m_layers) {

      // is layer collidable
      if (layer.second->canCollide()) {
        const auto maybeCollisions =
            layer.second->checkCollisions(*pUpdateObject);

        // is there a collision
        if (maybeCollisions.size() > 0) {
          std::optional<GameObject> maybeGameObject =
              layer.second->resolveCollisions(*pUpdateObject);
          CAP_THROW_ASSERT(maybeGameObject != std::nullopt,
                           "Collision could not be resolved.");

          pUpdateObject = maybeGameObject->clone();
        }
      }
    }

    // check collisions with other objects

    // continue with updated object if there were no collisions.
    i.reset(pUpdateObject.release());
  }
}

//! render function for the scene.
/**
 \param  in_windowId The id of the window to render to.
*/
void Scene2d::render(uint32_t in_windowId)
{
  // updateRenderLogicalSize(in_windowId);
  updateCameraSize(in_windowId, m_camera);

  // iterate in reverse order to draw layers in the proper order.
  for (auto i = m_layers.rbegin(); i != m_layers.rend(); ++i) {
    assert(i->second != nullptr);
    i->second->render(m_camera, in_windowId);
  }

  CAP_THROW_NULL(m_pObjectManager, "ObjectManager is null");
  // render objects
  for (auto &&pObject :
       m_pObjectManager->getObjects(m_camera.getViewingRectangle())) {
    assert(pObject != nullptr);
    pObject->render(m_camera, in_windowId);
  }
}
} // namespace CapEngine

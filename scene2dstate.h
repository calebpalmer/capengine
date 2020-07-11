#ifndef CAPENGINE_SCENE2DSTATE_H
#define CAPENGINE_SCENE2DSTATE_H

#include "CapEngineException.h"
#include "gamestate.h"
#include "scene2d.h"
#include "simpleobjectmanager.h"

#include <jsoncons/json.hpp>

namespace CapEngine
{

//! Exception class for scene that does not exist.
struct SceneDoesNotExistException : public CapEngineException {
  explicit SceneDoesNotExistException(const std::string &in_scenedId)
      : CapEngineException(std::string("Scene does not exist: ") + in_scenedId)
  {
  }
};

//! Game State for 2d scenes.
class Scene2dState final : public GameState
{
public:
  Scene2dState(jsoncons::json in_sceneDescriptors, std::string in_sceneId,
               uint32_t in_windowId);

  ~Scene2dState() override = default;
  void render() override;
  void update(double ms) override;
  bool onLoad() override;
  bool onDestroy() override;

private:
  using ObjectManagerType = SimpleObjectManager;

  //! Descriptors of all the available scenes.
  jsoncons::json m_sceneDescriptors;
  //! The id of the scene to load.
  std::string m_sceneId;
  //! The scene.
  std::unique_ptr<Scene2d<ObjectManagerType>> m_pScene;
  //! The id of the window to render to.
  uint32_t m_windowId;
};
} // namespace CapEngine

#endif // CAPENGINE_SCENE2DSTATE_H

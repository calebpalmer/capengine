#include "scene2dstate.h"

#include "scene2dschema.h"
#include "simpleobjectmanager.h"

namespace CapEngine {


//! Constructor.
/** 
 \param in_sceneDescriptors
   The scene descriptors.
 \param in_sceneID
   The id of the scene to load.
 \param
   The id of the window to use.
*/
Scene2dState::Scene2dState(jsoncons::json in_sceneDescriptors, std::string in_sceneId, uint32_t in_windowId)
	: m_sceneDescriptors(std::move(in_sceneDescriptors)), m_sceneId(std::move(in_sceneId)), m_windowId(in_windowId)
{
	using namespace Schema::Scene2d;
	
	for(auto && scene : m_sceneDescriptors[kScenes].array_range()){
		if(scene[kSceneId] == m_sceneId){
			m_pScene.reset(new Scene2d<SimpleObjectManager>(scene));
		}
	}

	if(m_pScene == nullptr){
		BOOST_THROW_EXCEPTION(SceneDoesNotExistException(in_sceneId));
	}
}

//! \copydoc GameState::onLoad
bool Scene2dState::onLoad(){
	return true;
}

//! \copydoc GameState::onDestroy
bool Scene2dState::onDestroy(){
	return true;
}

//! \copydoc GameState::render
void Scene2dState::render() {
	assert(m_pScene != nullptr);
	m_pScene->render(m_windowId);
}

//! \copydoc Gamestate::update
void Scene2dState::update(double ms){
	assert(m_pScene != nullptr);
	m_pScene->update(ms);
}

}

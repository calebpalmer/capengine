namespace CapEngine {

//! Constructor
/** 
 \param in_json
   The json describing the scene.
*/
template <class ObjectManager>
Scene2d<ObjectManager>::Scene2d(const jsoncons::json &in_json)
{
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

}

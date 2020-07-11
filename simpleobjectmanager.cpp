#include "simpleobjectmanager.h"

namespace CapEngine
{

//! Gets the game objects held by this object manager.
/**
  \return
          The objects.
*/
std::vector<std::shared_ptr<GameObject>> &SimpleObjectManager::getObjects()
{
  return m_objects;
}

//! Gets any objects that intersect a rectangle.
/**
 \param in_rectangle
   The rectangle.
 \return
   The objects that intersect the rectangle.
*/
std::vector<std::shared_ptr<GameObject>>
    SimpleObjectManager::getObjects(const Rectangle &in_rectangle)
{
  std::vector<std::shared_ptr<GameObject>> ret;

  for (auto &&pObject : m_objects) {
    Relation relation = MBRRelate(pObject->boundingPolygon(), in_rectangle);
    if (relation == TOUCH || relation == INSIDE) {
      ret.push_back(pObject);
    }
  }

  return ret;
}

//! Returns a vector of collisions if there are any.
/**
 \return
   The vector of collisions.
*/
std::vector<CollisionEvent> SimpleObjectManager::getCollisions() const
{
  return std::vector<CollisionEvent>{};
}

//! Adds an object to the object manager.
/**
 \param in_pObject
   \li The object to add.
*/
void SimpleObjectManager::addObject(std::shared_ptr<GameObject> in_pObject)
{
  m_objects.push_back(std::move(in_pObject));
}

} // namespace CapEngine

#include "simpleobjectmanager.h"
#include "CapEngineException.h"
#include "collision.h"

#include <iostream>

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
        CAP_THROW_NULL(pObject, "Object is null");
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
    std::vector<CollisionEvent> collisions;
    auto currentObject = this->m_objects.begin();

    if (currentObject != m_objects.end()) {
        CAP_THROW_ASSERT(*currentObject != nullptr, "currentObject is null.");
        while (currentObject != this->m_objects.end()) {
            // compare this object with the rest of the objects
            auto otherObject = currentObject++;
            while (otherObject != this->m_objects.end()) {
                CAP_THROW_ASSERT(*otherObject != nullptr,
                                 "otherObject is null.");

                CollisionType collisionType =
                    detectMBRCollision((*currentObject)->boundingPolygon(),
                                       (*otherObject)->boundingPolygon());

                if (collisionType != CollisionType::COLLISION_NONE) {
                    collisions.push_back(CollisionEvent{
                        *currentObject, *otherObject, collisionType,
                        CollisionClass::COLLISION_ENTITY});
                }

                otherObject++;
            }
        }
    }

    return collisions;
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

/**
 * @brief Deletes dead objects.
 */
void SimpleObjectManager::removeDeadObjects()
{
    const auto newEnd = std::remove_if(
        m_objects.begin(), m_objects.end(), [](const auto &object) {
            return object->getObjectState() == GameObject::Dead;
        });
    m_objects.erase(newEnd, m_objects.end());
}

} // namespace CapEngine

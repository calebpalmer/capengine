#ifndef CAPENGINE_OBJECTMANAGER_H
#define CAPENGINE_OBJECTMANAGER_H

#include "gameobject.h"

namespace CapEngine
{

class ObjectManager
{
  public:
    virtual ~ObjectManager() = default;

    virtual std::vector<std::shared_ptr<GameObject>> &getObjects() = 0;
    virtual std::vector<std::shared_ptr<GameObject>>
        getObjects(const Rectangle &in_rectangle) = 0;

    virtual std::vector<CollisionEvent> getCollisions() const = 0;

    virtual void addObject(std::shared_ptr<GameObject> in_pObject) = 0;
    virtual void removeDeadObjects() = 0;

    static constexpr char kObjectManagerLocatorId[] = "ObjectManager";
};

/**
   \fn ObjectManager::getObjects
   \brief Gets the objects in the ObjectManager.
   \return The objects in the ObjectManager.
 */

/**
   \fn ObjectManager::getObjects(const Rectangle&)
   \brief Gets the objects in the ObjectManager.
   \param in_rectangle
   \li The rectangle that contains the objects to get.
   \return The objects in the ObjectManager.
 */

/**
   \fn ObjectManager::getObjects(const Rectangle&)
   \brief Gets the objects in the ObjectManager.
   \param in_rectangle
   \li The rectangle that contains the objects to get.
   \return The objects in the ObjectManager.
 */

/**
   \fn ObjectManager::getCollisions()
   \brief Gets the collisions in the ObjectManager.
   \return The collisions.
 */

/**
   \fn ObjectManager::addObject
   \brief Add an object to the object manager.
   \param in_pObject
   \li The object to add.
 */

} // namespace CapEngine

#endif /* CAPENGINE_OBJECTMANAGER_H */

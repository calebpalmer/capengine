#ifndef CAPENGINE_SIMPLEOBJECTMANAGER_H
#define CAPENGINE_SIMPLEOBJECTMANAGER_H

#include "collision.h"
#include "gameobject.h"
#include "objectmanager.h"

namespace CapEngine
{

//! Simple manager for game objects
class SimpleObjectManager final : public ObjectManager
{
  public:
    std::vector<std::shared_ptr<GameObject>> &getObjects() override;
    std::vector<std::shared_ptr<GameObject>>
        getObjects(const Rectangle &in_rectangle) override;

    std::vector<CollisionEvent> getCollisions() const override;

    void addObject(std::shared_ptr<GameObject> in_pObject) override;
    void removeDeadObjects() override;

  private:
    //! Holds the objects
    std::vector<std::shared_ptr<GameObject>> m_objects;
};

} // namespace CapEngine

#endif // CAPENGINE_SIMPLEOBJECTMANAGER_H

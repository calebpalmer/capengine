#ifndef CAPENGINE_SIMPLEOBJECTMANAGER_H
#define CAPENGINE_SIMPLEOBJECTMANAGER_H

#include "gameobject.h"
#include "collision.h"

namespace CapEngine {

  //! Simple manager for game objects
  class SimpleObjectManager final {
  public:
    std::vector<std::shared_ptr<GameObject>>& getObjects();
    std::vector<std::shared_ptr<GameObject>> getObjects(const Rectangle &in_rectangle);    

    std::vector<CollisionEvent> getCollisions() const;

    void addObject(std::shared_ptr<GameObject> in_pObject);

  private:
    //! Holds the objects
    std::vector<std::shared_ptr<GameObject>> m_objects;
  };

}

#endif // CAPENGINE_SIMPLEOBJECTMANAGER_H
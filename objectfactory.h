#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

#include "gameobject.h"

#include <memory>
#include <string>
#include <map>
#include <functional>

namespace CapEngine{

  class ObjectFactory{
  public:
    static ObjectFactory& getInstance();
    std::shared_ptr<GameObject> makeObject(const std::string& objectType);
    void registerFactoryFunction(const std::string& objectType,
			std::function<std::shared_ptr<GameObject> (void)> function);
    

  protected:
    ObjectFactory() = default;
    std::function<std::shared_ptr<GameObject>()> getFactoryFunction(const std::string& objectType);

    static ObjectFactory* m_pObjectFactory;
    std::map<std::string, std::function<std::shared_ptr<GameObject>()>> m_functions;

  };
}

#endif // OBJECTFACTORY_H

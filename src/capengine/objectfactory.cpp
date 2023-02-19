#include "objectfactory.h"

#include "CapEngineException.h"

#include <cassert>
#include <sstream>

using namespace CapEngine;

ObjectFactory *ObjectFactory::m_pObjectFactory = nullptr;

ObjectFactory &ObjectFactory::getInstance()
{
  if (m_pObjectFactory == nullptr) {
    m_pObjectFactory = new ObjectFactory();
  }

  return *m_pObjectFactory;
}

std::function<std::shared_ptr<GameObject>()>
    ObjectFactory::getFactoryFunction(const std::string &objectType)
{
  auto factoryFunction = m_functions.find(objectType);
  if (factoryFunction == m_functions.end()) {
    std::ostringstream expStream;
    expStream << "Unknown Factory Function: " << objectType;
    throw CapEngineException(expStream.str());
  }

  return factoryFunction->second;
}

void ObjectFactory::registerFactoryFunction(
    const std::string &objectType,
    std::function<std::shared_ptr<GameObject>()> inFunction)
{
  m_functions[objectType] = inFunction;
}

std::shared_ptr<GameObject>
    ObjectFactory::makeObject(const std::string &objectType)
{
  auto factoryFunction = getFactoryFunction(objectType);
  assert(factoryFunction != nullptr);

  return factoryFunction();
}

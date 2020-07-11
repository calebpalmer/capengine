#include "gameobject.h"

#include "locator.h"
#include "logger.h"

#include <cmath>
#include <iostream>
#include <sstream>

namespace CapEngine
{
using namespace std;

ObjectID GameObject::nextID = 0;
int GameObject::nextMessageId = 0;

GameObject::GameObject(bool newID)
{
  if (newID) {
    m_objectID = generateID();
  }
  m_parentObjectID = -1;
}

/**
   Swap 2 game objects.

   \param io_other
   The other object.
*/
void GameObject::swap(GameObject &io_other) noexcept
{
  if (this != &io_other) {
    std::swap(position, io_other.position);
    std::swap(previousPosition, io_other.previousPosition);
    std::swap(orientation, io_other.orientation);
    std::swap(velocity, io_other.velocity);
    std::swap(acceleration, io_other.acceleration);
    std::swap(force, io_other.force);
    std::swap(m_pObjectData, io_other.m_pObjectData);
    std::swap(m_objectState, io_other.m_objectState);
    std::swap(m_objectID, io_other.m_objectID);
    std::swap(m_parentObjectID, io_other.m_parentObjectID);
    std::swap(m_objectType, io_other.m_objectType);
    std::swap(m_components, io_other.m_components);
  }
}

//! Copy constructor.
GameObject::GameObject(const GameObject &in_other)
{
  position = in_other.position;
  previousPosition = in_other.previousPosition;
  orientation = in_other.orientation;
  velocity = in_other.velocity;
  acceleration = in_other.acceleration;
  force = in_other.force;

  // darn, I don't have a clone for this polymorphic type.
  // pretty sure I'm not doing much with it though.
  m_pObjectData = in_other.m_pObjectData;

  m_objectState = in_other.m_objectState;
  m_objectID = generateID();
  m_parentObjectID = in_other.m_parentObjectID;
  m_objectType = in_other.m_objectType;

  // deep copy the componentsn
  m_components.reserve(in_other.m_components.size());
  std::transform(
      in_other.m_components.begin(), in_other.m_components.end(),
      std::back_inserter(m_components),
      [](std::shared_ptr<Component> in_pComponent)
          -> std::shared_ptr<Component> { return in_pComponent->clone(); });
}

GameObject::GameObject(GameObject &&in_other)
{
  position = in_other.position;
  previousPosition = in_other.previousPosition;
  orientation = in_other.orientation;
  velocity = in_other.velocity;
  acceleration = in_other.acceleration;
  force = in_other.force;

  // darn, I don't have a clone for this polymorphic type.
  // pretty sure I'm not doing much with it though.
  m_pObjectData = std::move(in_other.m_pObjectData);

  m_objectState = in_other.m_objectState;
  m_objectID = generateID();
  m_parentObjectID = in_other.m_parentObjectID;
  m_objectType = in_other.m_objectType;

  // deep copy the components
  m_components = std::move(in_other.m_components);
}

GameObject &GameObject::operator=(GameObject &&in_other)
{
  if (this != &in_other) {
    this->position = in_other.position;
    this->previousPosition = in_other.previousPosition;
    this->orientation = in_other.orientation;
    this->velocity = in_other.velocity;
    this->acceleration = in_other.acceleration;
    this->force = in_other.force;

    // darn, I don't have a clone for this polymorphic type.
    // pretty sure I'm not doing much with it though.
    this->m_pObjectData = std::move(in_other.m_pObjectData);

    this->m_objectState = in_other.m_objectState;
    this->m_objectID = generateID();
    this->m_parentObjectID = in_other.m_parentObjectID;
    this->m_objectType = in_other.m_objectType;

    // deep copy the components
    this->m_components = std::move(in_other.m_components);
  }

  return *this;
}

//! Copy assignment operator.
GameObject &GameObject::operator=(const GameObject &in_other)
{
  GameObject tmp(in_other);
  swap(tmp);
  return *this;
}

void GameObject::render(const Camera2d &in_camera, uint32_t in_windowId)
{
  std::vector<std::shared_ptr<Component>> graphicsComponents =
      this->getComponents(ComponentType::Graphics);

  for (auto &&pComponent : graphicsComponents) {
    auto pGraphicsComponent =
        std::dynamic_pointer_cast<GraphicsComponent>(pComponent);
    assert(pGraphicsComponent != nullptr);

    pGraphicsComponent->render(*this, in_camera, in_windowId);
  }
}

unique_ptr<GameObject> GameObject::update(double ms) const
{

  // clone new game object and pas to updates
  unique_ptr<GameObject> newObject = this->clone();

  for (auto &&pComponent : newObject->getComponents()) {
    pComponent->update(*newObject, ms);
  }

  return newObject;
}

Rectangle GameObject::boundingPolygon() const
{
  Rectangle rectangle;
  bool first = true;

  for (auto &&pComponent : m_components) {
    auto pPhysicsComponent =
        std::dynamic_pointer_cast<PhysicsComponent>(pComponent);
    if (pPhysicsComponent) {
      std::optional<Rectangle> maybeRectangle =
          pPhysicsComponent->boundingPolygon(*this);
      if (!maybeRectangle) {
        continue;
      }

      if (first) {
        rectangle = *maybeRectangle;
        first = false;
      }

      else {
        rectangle = join(rectangle, *maybeRectangle);
      }
    }
  }

  if (first) {
    // no collider was found.  make a 1x1 rect based off position.
    return Rectangle{static_cast<int>(std::round(position.getX())),
                     static_cast<int>(std::round(position.getY())), 1, 1};
  }

  return rectangle;
}

bool GameObject::handleCollision(CapEngine::CollisionType type,
                                 CapEngine::CollisionClass class_,
                                 GameObject *otherObject,
                                 Vector collisionLocation)
{
  return false;
}

unique_ptr<GameObject> GameObject::clone() const
{

  unique_ptr<GameObject> newObj(new GameObject(false));

  newObj->position = position;
  newObj->previousPosition = previousPosition;
  newObj->orientation = orientation;
  newObj->velocity = velocity;
  newObj->acceleration = acceleration;
  newObj->force = force;

  newObj->m_components.reserve(m_components.size());
  std::transform(m_components.begin(), m_components.end(),
                 std::back_inserter(newObj->m_components),
                 [](auto &&pComponent) -> std::unique_ptr<Component> {
                   return pComponent->clone();
                 });

  newObj->m_pObjectData = m_pObjectData;
  newObj->m_objectState = m_objectState;
  newObj->m_objectID = m_objectID;
  newObj->m_parentObjectID = m_parentObjectID;

  return newObj;
}

ObjectID GameObject::generateID()
{
  ostringstream msg;
  msg << "ID " << (nextID) << " generated";
  Locator::logger->log(msg.str(), Logger::CDEBUG, __FILE__, __LINE__);
  return nextID++;
}

std::shared_ptr<ObjectData> GameObject::getObjectData() const
{
  return m_pObjectData;
}

void GameObject::setObjectData(std::shared_ptr<ObjectData> pObjectData)
{
  m_pObjectData = pObjectData;
}

GameObject::ObjectState GameObject::getObjectState() const
{
  return m_objectState;
}

void GameObject::setObjectState(GameObject::ObjectState objectState)
{
  m_objectState = objectState;
}

ObjectID GameObject::getObjectID() const { return m_objectID; }

void GameObject::setObjectID(ObjectID id) { m_objectID = id; }

ObjectID GameObject::getParentObjectID() const { return m_parentObjectID; }

void GameObject::setParentObjectID(ObjectID id) { m_parentObjectID = id; }

Vector const &GameObject::getPosition() const { return position; }

void GameObject::setPosition(Vector positionIn) { position = positionIn; }

Vector const &GameObject::getOrientation() const { return orientation; }

void GameObject::setOrientation(Vector orientationIn)
{
  orientation = orientationIn;
}

Vector const &GameObject::getVelocity() const { return velocity; }

void GameObject::setVelocity(Vector velocityIn) { velocity = velocityIn; }

Vector const &GameObject::getAcceleration() const { return acceleration; }

void GameObject::setAcceleration(Vector accelerationIn)
{
  acceleration = accelerationIn;
}

Vector const &GameObject::getForce() const { return force; }

void GameObject::setForce(Vector in_force) { this->force = in_force; }

int GameObject::generateMessageId() { return nextMessageId++; }

void GameObject::send(int id, string message)
{
  for (auto &&pComponent : m_components) {
    assert(pComponent != nullptr);
    pComponent->receive(*this, id, message);
  }
}

/**
   Return the previous position of the object
*/
Vector const &GameObject::getPreviousPosition() const
{
  return previousPosition;
}

/**
   Set the previous position of the object
*/
void GameObject::setPreviousPosition(Vector position)
{
  previousPosition = position;
}

std::ostream &operator<<(std::ostream &stream,
                         const CollisionEvent collisionEvent)
{
  std::ostringstream repr;
  repr << collisionEvent.type << " with "
       << collisionEvent.object1->getObjectID();
  if (collisionEvent.object2 != nullptr)
    repr << " and " << collisionEvent.object2->getObjectID();

  repr << " class: " << collisionEvent.class_;

  stream << repr.str();
  return stream;
}

std::ostream &operator<<(std::ostream &stream, GameObject const &gameObject)
{
  std::ostringstream repr;
  repr << "GameObject[" << gameObject.m_objectID << "]@"
       << gameObject.boundingPolygon();
  ;

  stream << repr.str();
  return stream;
}

GameObject::ObjectType GameObject::getObjectType() const
{
  return m_objectType;
}

void GameObject::setObjectType(GameObject::ObjectType in_objectType)
{
  m_objectType = in_objectType;
}

//! Adds a new component
/**
    \param in_component
    The component.
*/
void GameObject::addComponent(std::shared_ptr<Component> in_pComponent)
{
  if (in_pComponent == nullptr) {
    BOOST_THROW_EXCEPTION(CapEngineException("Null component"));
  }

  m_components.emplace_back(std::move(in_pComponent));
}

//! Gets the components.
/**
    \return
    The components
*/
const std::vector<std::shared_ptr<Component>> &GameObject::getComponents()
{
  return m_components;
}

//! Gets all the components of a given type.
/**
    \param in_type
    The type
    \return
    The components.
*/
std::vector<std::shared_ptr<Component>>
    GameObject::getComponents(ComponentType in_type)
{
  std::vector<std::shared_ptr<Component>> components;

  std::copy_if(m_components.begin(), m_components.end(),
               std::back_inserter(components), [in_type](auto &&in_component) {
                 return in_component->getType() == in_type;
               });

  return components;
}

} // namespace CapEngine

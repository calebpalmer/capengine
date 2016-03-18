#include "gameobject.h"

#include "logger.h"
#include "locator.h"

#include <iostream>
#include <sstream>

using namespace CapEngine;
using namespace std;

ObjectID GameObject::nextID = 0;

GameObject::GameObject(bool newID) {
  if(newID){
    m_objectID = generateID();
  }
  m_parentObjectID = -1;
}

GameObject::~GameObject(){ }

void GameObject::render(){
  graphicsComponent->render(this);
}

unique_ptr<GameObject> GameObject::update(double ms) const{

  // clone new game object and pas to updates
  //  unique_ptr<GameObject> newObj(new GameObject(*this));
  unique_ptr<GameObject> newObj = this->clone();
  //  *newObj = *this;

  if(newObj->inputComponent){
    newObj->inputComponent->update(newObj.get());
  }
  if(newObj->physicsComponent){
    newObj->physicsComponent->update(newObj.get(), ms);
  }
  if(newObj->customComponent){
    newObj->customComponent->update(newObj.get());
  }
  if(newObj->mpAIComponent){
    newObj->mpAIComponent->update(newObj.get());
  }

  return move(newObj);
}

Rectangle GameObject::boundingPolygon() const {
  return physicsComponent->boundingPolygon(this);
}


bool GameObject::handleCollision(CapEngine::CollisionType type, CapEngine::CollisionClass class_, GameObject* otherObject){
  return physicsComponent->handleCollision(this, type, class_, otherObject);
}

unique_ptr<GameObject> GameObject::clone() const{
  unique_ptr<GameObject> newObj(new GameObject(false));

  newObj->position = position;
  newObj->orientation = orientation;
  newObj->velocity = velocity;
  if(inputComponent){
    newObj->inputComponent = inputComponent;
  }
  if(physicsComponent){
    newObj->physicsComponent = physicsComponent;
  }
  if(graphicsComponent){
    newObj->graphicsComponent = graphicsComponent;
  }
  if(customComponent){
    newObj->customComponent.reset(customComponent->clone());
  }
  if(mpAIComponent){
    newObj->mpAIComponent.reset(mpAIComponent->clone());
  }
  newObj->m_pObjectData = m_pObjectData;
  newObj->m_objectState = m_objectState;
  newObj->m_objectID = m_objectID;
  newObj->m_parentObjectID = m_parentObjectID;

  return std::move(newObj);
}

ObjectID GameObject::generateID(){
  ostringstream msg;
  msg << "ID " << (nextID) << " generated";
  Locator::logger->log(msg.str(), Logger::CDEBUG);
  return nextID++;
}

std::shared_ptr<ObjectData> GameObject::getObjectData() const {
  return m_pObjectData;
}

void GameObject::setObjectData(std::shared_ptr<ObjectData> pObjectData){
  m_pObjectData = pObjectData;
}

GameObject::ObjectState GameObject::getObjectState() const{
  return m_objectState;
}

void GameObject::setObjectState(GameObject::ObjectState objectState){
  m_objectState = objectState;
}

ObjectID GameObject::getObjectID() const{
  return m_objectID;
}

void GameObject::setObjectID(ObjectID id){
  m_objectID = id;
}

ObjectID GameObject::getParentObjectID() const{
  return m_parentObjectID;
}

void GameObject::setParentObjectID(ObjectID id){
  m_parentObjectID = id;
}

Vector GameObject::getPosition() const{
  return position;
}

void GameObject::setPosition(Vector positionIn){
  position = positionIn;
}

Vector GameObject::getOrientation() const{
  return orientation;
}

void GameObject::setOrientation(Vector orientationIn){
  orientation = orientationIn;
}

Vector GameObject::getVelocity() const{
  return velocity;
}

void GameObject::setVelocity(Vector velocityIn){
  velocity = velocityIn;
}

void GameObject::setInputComponent(shared_ptr<InputComponent> pInputComponent){
  inputComponent = pInputComponent;
}

void GameObject::setPhysicsComponent(shared_ptr<PhysicsComponent> pPhysicsComponent){
  physicsComponent = pPhysicsComponent;
}

void GameObject::setGraphicsComponent(shared_ptr<GraphicsComponent> pGraphicsComponent){
  graphicsComponent = pGraphicsComponent;
}

void GameObject::setCustomComponent(shared_ptr<CustomComponent> pCustomComponent){
  customComponent = pCustomComponent;
}

void GameObject::setAIComponent(shared_ptr<AIComponent> pAIComponentIn){
  mpAIComponent = pAIComponentIn;
}

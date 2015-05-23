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
  newObj->objectType = objectType;
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

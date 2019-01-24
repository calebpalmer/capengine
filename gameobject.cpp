#include "gameobject.h"

#include "logger.h"
#include "locator.h"

#include <iostream>
#include <sstream>

namespace CapEngine {
using namespace std;

ObjectID GameObject::nextID = 0;
int GameObject::nextMessageId = 0;

GameObject::GameObject(bool newID) {
	if(newID){
		m_objectID = generateID();
	}
	m_parentObjectID = -1;
}

void GameObject::render(const Camera2d &in_camera){
	graphicsComponent->render(this, in_camera);
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
	if(newObj->graphicsComponent){
		newObj->graphicsComponent->update(newObj.get(), ms);
	}

	return move(newObj);
}

Rectangle GameObject::boundingPolygon() const {
	return physicsComponent->boundingPolygon(this);
}


bool GameObject::handleCollision(CapEngine::CollisionType type, CapEngine::CollisionClass class_, GameObject* otherObject,
																 Vector collisionLocation){
	return physicsComponent->handleCollision(this, type, class_, otherObject, collisionLocation);
}

unique_ptr<GameObject> GameObject::clone() const{
	unique_ptr<GameObject> newObj(new GameObject(false));

	newObj->position = position;
	newObj->previousPosition = previousPosition;
	newObj->orientation = orientation;
	newObj->velocity = velocity;
	newObj->acceleration = acceleration;
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
		newObj->customComponent.reset(customComponent->clone().release());
	}
	if(mpAIComponent){
		newObj->mpAIComponent.reset(mpAIComponent->clone().release());
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
	Locator::logger->log(msg.str(), Logger::CDEBUG, __FILE__, __LINE__);
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

Vector GameObject::getAcceleration() const{
	return acceleration;
}

void GameObject::setAcceleration(Vector accelerationIn){
	acceleration = accelerationIn;
}

//!  Sets the Component on the GameObject
/** 
 \param pComponent
   The Component to set.
*/
void GameObject::setComponent(std::shared_ptr<Component> pComponent){
	if(auto pInputComponent = std::dynamic_pointer_cast<InputComponent>(pComponent)){
		this->setInputComponent(pInputComponent);
	}

	else if(auto pPhysicsComponent = std::dynamic_pointer_cast<PhysicsComponent>(pComponent)){
		this->setPhysicsComponent(pPhysicsComponent);
	}

	else if(auto pGraphicsComponent = std::dynamic_pointer_cast<GraphicsComponent>(pComponent)){
		this->setGraphicsComponent(pGraphicsComponent);
	}

	else if(auto pAIComponent = std::dynamic_pointer_cast<AIComponent>(pComponent)){
		this->setAIComponent(pAIComponent);
	}

	else if(auto pCustomComponent = std::dynamic_pointer_cast<CustomComponent>(pComponent)){
		this->setCustomComponent(pCustomComponent);
	}

}

void GameObject::setInputComponent(shared_ptr<InputComponent> pInputComponent){
	inputComponent = pInputComponent;
}

void GameObject::setPhysicsComponent(shared_ptr<PhysicsComponent> pPhysicsComponent){
	physicsComponent = pPhysicsComponent;
}

/**
	 Accessor method for objects PhysicsComponent
*/
std::shared_ptr<PhysicsComponent> GameObject::getPhysicsComponent() const {
	return physicsComponent;
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

int GameObject::generateMessageId(){
	return nextMessageId++;
}

void GameObject::send(int id, string message){
	// ostringstream messageStr;
	// messageStr << *this << ": Message: " << message;
	// Locator::logger->log(messageStr.str(), Logger::CDEBUG, __FILE__, __LINE__);
    
	if(inputComponent){
		inputComponent->receive(this, id, message);
	}
	if(physicsComponent){
		physicsComponent->receive(this, id, message);
	}
	if(graphicsComponent){
		graphicsComponent->receive(this, id, message);
	}
	if(customComponent){
		customComponent->receive(this, id, message);
	}
	if(mpAIComponent){
		mpAIComponent->receive(this, id, message);
	}
}

/**
	 Return the previous position of the object
*/
Vector GameObject::getPreviousPosition() const{
	return previousPosition;
}

/**
	 Set the previous position of the object
*/
void GameObject::setPreviousPosition(Vector position){
	previousPosition = position;
}

std::shared_ptr<InputComponent> GameObject::getInputComponent(){
	return inputComponent;
}


std::ostream& operator<<(std::ostream& stream, const CollisionEvent collisionEvent){
	std::ostringstream repr;
	repr << collisionEvent.type << " with " << collisionEvent.object1->getObjectID();
	if(collisionEvent.object2 != nullptr)
		repr << " and " << collisionEvent.object2->getObjectID();

	repr << " class: " << collisionEvent.class_;

	stream << repr.str();
	return stream;
}


std::ostream& operator<<(std::ostream& stream, GameObject const & gameObject){
	std::ostringstream repr;
	repr << "GameObject[" << gameObject.m_objectID << "]@" << gameObject.boundingPolygon();;

	stream << repr.str();
	return stream;
}

GameObject::ObjectType GameObject::getObjectType() const{
	return m_objectType;
}

void GameObject::setObjectType(GameObject::ObjectType in_objectType){
	m_objectType = in_objectType;
}
  
}


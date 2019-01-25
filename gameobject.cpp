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

int GameObject::generateMessageId(){
	return nextMessageId++;
}

void GameObject::send(int id, string message){
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

//! Adds a new component
/** 
 \param in_component
   The component.
*/
void GameObject::addComponent(std::shared_ptr<Component> in_pComponent){
	if(in_pComponent == nullptr){
		BOOST_THROW_EXCEPTION(CapEngineException("Null component"));
	}

	m_components.emplace_back(std::move(in_pComponent));
}

//! Gets the components.
/** 
 \return 
   The components
*/
const std::vector<std::shared_ptr<Component>>& GameObject::getComponents(){
	return m_components;
}

//! Gets all the components of a given type.
/** 
 \param in_type
   The type
 \return 
   The components.
*/
std::vector<std::shared_ptr<Component>> GameObject::getComponents(ComponentType in_type){
	std::vector<std::shared_ptr<Component>> components;

	std::copy_if(m_components.begin(), m_components.end(), std::back_inserter(components),
							 [in_type](auto && in_component)
							 {
								 return in_component->getType() == in_type;
							 });

	return components;
}

}


#include "property.h"

namespace CapEngine {

//! Constructor
/** 
 \param name
   The name of the property.
 \param value
   The value of the property.
 \param type
   The type of the property.
*/
Property::Property(std::string name, PropertyValueType value, PropertyType type) :
	m_name(std::move(name)), m_value(std::move(value)), m_type(type)
{
}

//!  Get the value.
/** 
 \return 
   The value of the property.
*/
Property::PropertyValueType Property::getValue() const {
	return m_value;
}


//! Gets the name.
/** 
 \return 
   The name.
*/
std::string Property::getName() const {
	return m_name;
}


//! Sets the name of the property.
/** 
	\param name
	  The name of the property.
*/
void Property::setName(std::string name){
	m_name = std::move(name);
}

//! Gets the type of the property.
/** 
 \return 
   The type.
*/
PropertyType Property::getType() const {
	return m_type;
}

//! accepts a PropertyVisitor object.
/** 
 \param vistor
   The visitor to call.
*/
void Property::accept(PropertyVisitor &visitor){
	if(m_type == PropertyType::String){
		visitor.visitString(*this);
	}
	else if(m_type == PropertyType::Int64){
		visitor.visitInt64(*this);
	}
	else if(m_type == PropertyType::UInt64){
		visitor.visitUInt64(*this);
	}
	else if(m_type == PropertyType::Int32){
		visitor.visitInt32(*this);
	}
	else if(m_type == PropertyType::UInt64){
		visitor.visitUInt64(*this);
	}
	else if(m_type == PropertyType::Float32){
		visitor.visitFloat32(*this);
	}
	else if(m_type == PropertyType::Float64){
		visitor.visitFloat64(*this);
	}	
}

}

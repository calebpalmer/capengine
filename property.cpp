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

}

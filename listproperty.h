#ifndef CAPENGINE_LISTPROPERTY_H
#define CAPENGINE_LISTPROPERTY_H

#include "property.h"
#include "propertytraits.h"

#include <optional>
#include <string_view>

namespace CapEngine {

//! A class for list properties
class ListProperty final {
public:
	using ListPropertyIterator = std::vector<Property>::iterator;
	using ConstListPropertyIterator = std::vector<Property>::const_iterator;
public:
	ListProperty() = default;
	ListProperty(std::vector<Property> properties);

	std::optional<Property> getProperty(std::string_view name) const;
	bool addProperty(Property property);
	std::optional<Property> removeProperty(std::string_view propertyName);

  ListPropertyIterator begin();
	ListPropertyIterator end();
  ConstListPropertyIterator cbegin() const;
	ConstListPropertyIterator cend() const;

	
private:
	std::vector<Property> m_properties;
		
};

namespace TypeTraits {

template <>
struct PropertyTraits<ListProperty>
{
	static const PropertyType propertyType = PropertyType::List;
};

}

}

#endif // CAPENGINE_LISTPROPERTY_H

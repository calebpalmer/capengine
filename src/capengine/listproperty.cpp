#include "listproperty.h"

namespace CapEngine
{

//! Constructor
/**
 \param properties
   The properties to initialize the list with.
*/
ListProperty::ListProperty(std::vector<Property> properties)
    : m_properties(std::move(properties))
{
}

//! Gets a property.
/**
 \param name
   The name of the propety to get.
 \return
   The property if found, std::nullopt otherwise.
*/
std::optional<Property> ListProperty::getProperty(std::string_view name) const
{
  auto &&property = std::find_if(
      m_properties.cbegin(), m_properties.cend(),
      [&name](const auto &property) { return property.getName() == name; });

  if (property != m_properties.end()) {
    return *property;
  }

  return std::nullopt;
}

//! Adds a property to the list.
/**
 \param property
   The propert.
 \return
   true if the property could be added, false if not.
*/
bool ListProperty::addProperty(Property property)
{
  if (this->getProperty(property.getName()) != std::nullopt) {
    m_properties.emplace_back(std::move(property));
    return true;
  }

  return false;
}

//! Removes a property from the list.
/**
 \param propertyName
   The name of the property to remove.
 \return
   The property if it could be removed, std::nullopt otherwise.
*/
std::optional<Property>
    ListProperty::removeProperty(std::string_view propertyName)
{
  auto &&property = std::find_if(m_properties.cbegin(), m_properties.cend(),
                                 [&propertyName](const auto &property) {
                                   return property.getName() == propertyName;
                                 });

  if (property != m_properties.end()) {
    std::optional<Property> retval = *property;
    m_properties.erase(property);
    return retval;
  }

  return std::nullopt;
}

//! Return an iterator to the first property in the list.
/**
 \return
   The iterator.
*/
ListProperty::ListPropertyIterator ListProperty::begin()
{
  return m_properties.begin();
}

//! Return an iterator to the last property in the list.
/**
 \return
   The iterator.
*/
ListProperty::ListPropertyIterator ListProperty::end()
{
  return m_properties.end();
}

//! Return an const iterator to the first property in the list.
/**
 \return
   The iterator.
*/
ListProperty::ConstListPropertyIterator ListProperty::cbegin() const
{
  return m_properties.cbegin();
}

//! Return an const iterator to the last property in the list.
/**
 \return
   The iterator.
*/
ListProperty::ConstListPropertyIterator ListProperty::cend() const
{
  return m_properties.cend();
}

} // namespace CapEngine

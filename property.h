#ifndef CAPENGINE_PROPERTY_H
#define CAPENGINE_PROPERTY_H

#include "propertycommon.h"
#include "propertytraits.h"
#include "propertyvisitor.h"

#include <boost/any.hpp>
#include <functional>
#include <memory>
#include <optional>

namespace CapEngine
{

//! Class to represent a property of something.
class Property final
{
public:
  template <typename T> static Property create(std::string name, T val);

  template <typename T>
  static std::unique_ptr<Property> createUP(std::string name, T val);

  boost::any getValue() const;
  template <typename T> std::optional<T> getValueAs() const;

  template <typename T> void setValue(T value);

  std::string getName() const;
  void setName(std::string name);

  PropertyType getType() const;
  void accept(PropertyVisitor &visitor);

  using OnChangeCB = std::function<void(const Property &oldProperty,
                                        const Property &newProperty)>;
  void addOnChangeCB(OnChangeCB f);
  void clearOnChangeCB();

private:
  Property(std::string name, boost::any value, PropertyType type);

  std::string m_name;                    //<! The name of the property.
  boost::any m_value;                    //<! The property value.
  PropertyType m_type;                   //<! The property type.
  std::vector<OnChangeCB> m_onChangeCBs; //<! collection of on change callbacks.
};

//! Factory for creating properties.
/**
                \param name
                The name of the property.
                \param value
                The value of the property.
                \return
                The property.
*/
template <typename T> Property Property::create(std::string name, T value)
{
  return Property{std::move(name), std::move(value),
                  TypeTraits::PropertyTraits<T>::propertyType};
}

//! \copydoc Property::create
template <typename T>
std::unique_ptr<Property> Property::createUP(std::string name, T value)
{
  return std::make_unique<Property>(
      std::move(name), std::move(value),
      TypeTraits::PropertyTraits<T>::propertyType);
}

//! Get the value as the given type.
/**
                \return
                The value if the given type, std::nullopt otherwise.
*/
template <typename T> std::optional<T> Property::getValueAs() const
{
  if (auto &&value = boost::any_cast<T>(&m_value)) {
    return *value;
  }

  return std::nullopt;
}

//!  Sets the value of the property.
/**
        \param value
                The value to set.
*/
template <typename T> void Property::setValue(T value)
{
  m_value = std::move(value);
  m_type = TypeTraits::PropertyTraits<T>::propertyType;
}
} // namespace CapEngine

#endif // CAPENGINE_PROPERTY_H

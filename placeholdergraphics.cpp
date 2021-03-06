#include "placeholdergraphics.h"

#include "VideoManager.h"
#include "componentfactory.h"
#include "componentutils.h"
#include "gameobject.h"
#include "locator.h"
#include "logger.h"
#include "scene2dschema.h"
#include "scene2dutils.h"

#include <cassert>
#include <cmath>
#include <jsoncons/json.hpp>

namespace CapEngine
{

//! Constructor
/**
    \param in_width
    The width.
    \param in_height
    The height.
    \param in_colour
    The colour
*/
PlaceHolderGraphics::PlaceHolderGraphics(int in_width, int in_height,
                                         Colour in_colour)
    : m_width(in_width), m_height(in_height), m_colour(std::move(in_colour))
{
}

//! \copydoc Component::cloneoi
std::unique_ptr<Component> PlaceHolderGraphics::clone() const
{
  return std::make_unique<PlaceHolderGraphics>(*this);
}

//! \copydoc GraphicsComponent::render
void PlaceHolderGraphics::render(GameObject &in_object,
                                 const Camera2d &in_camera,
                                 uint32_t in_windowId)
{
  const auto objectRect = in_object.boundingPolygon();
  const bool doYFlip =
      in_object.getYAxisOrientation() == YAxisOrientation::BottomZero;

  Rectangle drawRect =
      toScreenCoords(in_camera, objectRect, in_windowId, doYFlip);

  assert(Locator::videoManager != nullptr);
  Locator::videoManager->drawFillRect(in_windowId, drawRect.toRect(), m_colour);
}

//! Construct the component from Json.
/**
    \param in_json
    The json.
    \return
    The component.
*/
PlaceHolderGraphics
    PlaceHolderGraphics::makeComponent(const jsoncons::json &in_json)
{
  using namespace Schema::Components::PlaceHolderGraphics;
  try {
    assert(in_json[Schema::Components::kComponentSubType] == kType);

    int width = in_json[kWidth].as<int>();
    int height = in_json[kHeight].as<int>();
    Colour colour = JSONUtils::readColour(in_json[kColour]);
    return PlaceHolderGraphics{width, height, colour};
  } catch (const jsoncons::json_exception &e) {
    throw ComponentCreationException(ComponentType::Graphics, kType, in_json,
                                     boost::diagnostic_information(e));
  }
}

//! Registers a constructor with the object factory.
/**
    \param in_factory
    The factory.
*/
void PlaceHolderGraphics::registerConstructor(ComponentFactory &in_factory)
{
  auto constructor = [](const jsoncons::json &in_json)
      -> std::unique_ptr<PlaceHolderGraphics> {
    return std::make_unique<PlaceHolderGraphics>(makeComponent(in_json));
  };

  in_factory.registerComponentType(
      ComponentUtils::componentTypeToString(ComponentType::Graphics), kType,
      constructor);
}

} // namespace CapEngine

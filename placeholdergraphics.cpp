#include "placeholdergraphics.h"

#include "gameobject.h"
#include "VideoManager.h"
#include "locator.h"

#include <cmath>
#include <cassert>

namespace CapEngine {

//! Constructor
/** 
 \param in_width
   The width.
 \param in_height
   The height.
 \param in_colour
   The colour
*/
PlaceHolderGraphics::PlaceHolderGraphics(int in_width, int in_height, Colour in_colour)
	: m_width(in_width), m_height(in_height), m_colour(std::move(in_colour))
{

}

//! \copydoc Component::cloneoi
std::unique_ptr<Component> PlaceHolderGraphics::clone() const {
	return std::make_unique<PlaceHolderGraphics>(*this);
}

//! \copydoc GraphicsComponent::render
void PlaceHolderGraphics::render(GameObject *in_object, const Camera2d &in_camera, uint32_t in_windowId){
	assert(in_object != nullptr);
	Vector position = in_object->getPosition();
	Rectangle objectRect( position.getX() - std::round((static_cast<double>(m_width) / 2.0)),
												position.getY() - std::round((static_cast<double>(m_height) / 2.0)),
												m_width,
												m_height
												);

	Rectangle drawRect = toScreenCoords(in_camera, objectRect, in_windowId, true);

	assert(Locator::videoManager != nullptr);
	Locator::videoManager->drawFillRect(in_windowId, drawRect.toRect(), m_colour);
}

}

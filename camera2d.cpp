#include "camera2d.h"

#include "locator.h"
#include "CapEngineException.h"

namespace CapEngine {

//! Constructor
/** 
 \param in_width
   The width of the Camera view.
 \param in_height
   The height of the camera view.
*/
Camera2d::Camera2d(int in_width, int in_height)
	: m_viewRect(Rectangle(0, 0, in_width, in_height))
{
}

std::pair<int, int> Camera2d::getPosition() const {
	return std::make_pair(m_viewRect.x, m_viewRect.y);
}

//! Center camera on rectangle.
/** 
 \param in_rectangle
   The rectangle to center on.
 \param in_outerBounds
   An optional outer boundary rectangle that the camera cannot view outside.
*/
void Camera2d::center(const Rectangle &in_rectangle, std::optional<Rectangle> in_outerBounds){
	unsigned int xCenter = in_rectangle.x + (in_rectangle.width / 2);
	unsigned int yCenter = in_rectangle.y + (in_rectangle.height / 2);

	m_viewRect.x = xCenter - (m_viewRect.width / 2);
	m_viewRect.y = yCenter - (m_viewRect.height / 2);

	if(in_outerBounds != std::nullopt){
		// make sure it doesn't exceed the right hand side of the outer bounds
		int xExcess = (in_outerBounds->x + in_outerBounds->width) - (m_viewRect.x + m_viewRect.width);
		if(xExcess > 0){
			m_viewRect.x -= xExcess;
		}

		// make sure it doesn't exceed the left hand side of the outer bounds
		if(m_viewRect.x < in_outerBounds->x){
			m_viewRect.x = in_outerBounds->x;
		}

		// make sure it doesn't exceed the top of the outer bounds
		int yExcess = (in_outerBounds->y + in_outerBounds->height) - (m_viewRect.y + m_viewRect.height);
		if(yExcess > 0){
			m_viewRect.y -= yExcess;
		}

		// make sure it doesn't exceed the bottom of the outer bounds
		if(m_viewRect.y < in_outerBounds->y){
			m_viewRect.y = in_outerBounds->y;
		}
	}
}


//! Gets the viewing rectangle.
/** 
 \return 
   The viewing rectangle.
*/
const Rectangle& Camera2d::getViewingRectangle() const {
	return m_viewRect;
}

//! Transforms the given rectangle to screen coordinates.
/** 
 \param in_camera
   The camera.
 \param in_rect
   The rectangle to transform.
 \return 
*/
Rectangle toScreenCoords(const Camera2d &in_camera,
												 const Rectangle &in_rect,
												 uint32_t in_windowId,
												 bool doYFlip){
	
	const Rectangle& viewingRect = in_camera.getViewingRectangle();

	Rectangle rect(in_rect.x - viewingRect.x, in_rect.y - viewingRect.y, in_rect.width, in_rect.height);

	if(doYFlip){
		assert(Locator::videoManager != nullptr);

		if(!(Locator::videoManager->isValidWindowId(in_windowId))){
			std::stringstream msg;
			msg << "The window id " << in_windowId << " is not valid.";
			BOOST_THROW_EXCEPTION(CapEngineException(msg.str()));
		}

		int windowHeight = Locator::videoManager->getWindowHeight(in_windowId);
		rect.y = windowHeight - rect.y - 1;
	}

	return rect;
}

} // CapEngine

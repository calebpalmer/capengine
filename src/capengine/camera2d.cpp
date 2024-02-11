#include "camera2d.h"

#include "CapEngineException.h"
#include "locator.h"

namespace CapEngine
{

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

std::pair<int, int> Camera2d::getPosition() const
{
	return std::make_pair(m_viewRect.x, m_viewRect.y);
}

void Camera2d::setPosition(int in_x, int in_y)
{
	m_viewRect.x = in_x;
	m_viewRect.y = in_y;
}

//! Center camera on rectangle.
/**
 \param in_rectangle
   The rectangle to center on.
 \param in_outerBounds
   An optional outer boundary rectangle that the camera cannot view outside.
 \param in_point
   An optional point to center on.
*/
void Camera2d::center(const Rectangle &in_rectangle,
					  std::optional<Rectangle> in_outerBounds)
{
		unsigned int xCenter = in_rectangle.x + (in_rectangle.width / 2);
		unsigned int yCenter = in_rectangle.y + (in_rectangle.height / 2);

		m_viewRect.x = xCenter - (m_viewRect.width / 2);
		m_viewRect.y = yCenter - (m_viewRect.height / 2);

		if (in_outerBounds != std::nullopt) {
			// make sure it doesn't exceed the right hand side of the outer
			// bounds
			int xExcess = (in_outerBounds->x + in_outerBounds->width) -
						  (m_viewRect.x + m_viewRect.width);
			if (xExcess > 0) {
				m_viewRect.x -= xExcess;
			}

			// make sure it doesn't exceed the left hand side of the outer
			// bounds
			if (m_viewRect.x < in_outerBounds->x) {
				m_viewRect.x = in_outerBounds->x;
			}

			// make sure it doesn't exceed the top of the outer bounds
			int yExcess = (in_outerBounds->y + in_outerBounds->height) -
						  (m_viewRect.y + m_viewRect.height);
			if (yExcess > 0) {
				m_viewRect.y -= yExcess;
			}

			// make sure it doesn't exceed the bottom of the outer bounds
			if (m_viewRect.y < in_outerBounds->y) {
				m_viewRect.y = in_outerBounds->y;
			}
		}
}

//! Gets the viewing rectangle.
/**
 \return
   The viewing rectangle.
*/
const Rectangle &Camera2d::getViewingRectangle() const { return m_viewRect; }

//! Transforms the given rectangle to screen coordinates.
/**
 \param in_camera
   The camera.
 \param in_rect
   The rectangle to transform.
 \param in_windowWidth
   The window width.
 \param in_windowHeight
   The window height.
 \param doYFlip
   Flip the Y coords.
 \return
   The rectangle in screen coordinates.
*/
Rectangle toScreenCoords(const Camera2d &in_camera, const Rectangle &in_rect,
						 int /* in_windowWidth */, int in_windowHeight,
						 bool doYFlip)
{
		const Rectangle &viewingRect = in_camera.getViewingRectangle();

		Rectangle rect(in_rect.x - viewingRect.x, in_rect.y - viewingRect.y,
					   in_rect.width, in_rect.height);

		if (doYFlip) {
			rect.y = (in_windowHeight - rect.y) - rect.height;
		}

		// scaling out means decreasing the rectangle size by a scale
		// scaling in means increasing the rectangle size by a scale
		if (in_camera.zoom() != 1.0) {
			rect.width = rect.width * in_camera.zoom();
			rect.height = rect.height * in_camera.zoom();
		}

		return rect;
}

//! Transforms the given rectangle to screen coordinates.
/**
 \param in_camera
   The camera.
 \param in_rect
   The rectangle to transform.
 \param un_windowId
   The id of the window.
 \param doYFlip
   Flip the Y coords.
 \return
   The Vector in screen coordinates.
*/
Vector toScreenCoords(const Camera2d &in_camera, const Vector &in_point,
					  uint32_t in_windowId, bool doYFlip)
{
		auto [_, windowHeight] =
			Locator::videoManager->getWindowLogicalResolution(in_windowId);

		const Rectangle &viewingRect = in_camera.getViewingRectangle();

		Vector ret{in_point.x - static_cast<double>(viewingRect.x),
				   in_point.y - static_cast<double>(viewingRect.y)};

		if (doYFlip) {
			ret.y = (static_cast<double>(windowHeight) - ret.y);
		}

		return ret;
}

//! Transforms the given rectangle to screen coordinates.
/**
 \param in_camera
   The camera.
 \param in_rect
   The rectangle to transform.
 \param in_windowId
   The id of the window.
 \param doYFlip
   Flip the Y coords.
 \return
*/
Rectangle toScreenCoords(const Camera2d &in_camera, const Rectangle &in_rect,
						 uint32_t in_windowId, bool doYFlip)
{
		auto [windowWidth, windowHeight] =
			Locator::videoManager->getWindowLogicalResolution(in_windowId);

		return toScreenCoords(in_camera, in_rect, windowWidth, windowHeight,
							  doYFlip);
}

//! Get the width of the camera.
/**
 \return
   The width.
*/
int Camera2d::getWidth() const { return m_viewRect.width; }

//! Sets the width of the camera.
/**
 \param in_width
   The width of the camera.
*/
void Camera2d::setWidth(int in_width) { m_viewRect.width = in_width; }

//! Get the height of the camera.
/**
 \return
   The height.
*/
int Camera2d::getHeight() const { return m_viewRect.height; }

//! Sets the height of the camera.
/**
 \param in_height
   The height of the camera.
*/
void Camera2d::setHeight(int in_height) { m_viewRect.height = in_height; }

float Camera2d::zoom() const { return m_zoom; }

void Camera2d::zoom(float in_amount) { m_zoom += in_amount; }

void Camera2d::setZoom(float in_amount) { m_zoom = in_amount; }

//! Take a point in window coordates and transform it relative to in_rectangle
Vector windowToLocalCoordinate(Camera2d const& in_camera, Vector const& in_point, Rectangle const& in_rect, uint32_t windowId, bool doYFlip) {
	const Rectangle &viewingRect = in_camera.getViewingRectangle();

	int x = in_rect.x + viewingRect.x + static_cast<int>(in_point.x);
	int y = in_rect.y + viewingRect.y + static_cast<int>(in_point.y);

	auto [_, windowHeight] = Locator::videoManager->getWindowLogicalResolution(windowId);

	if (doYFlip) {
		y = (windowHeight - in_rect.y) - in_rect.height;
	}

	return Vector{static_cast<double>(x), static_cast<double>(y)};
}

} // namespace CapEngine

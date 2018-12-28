#ifndef CAPENGINE_CAMERA2D_H
#define CAPENGINE_CAMERA2D_H

#include "collision.h"

#include <optional>

namespace CapEngine {

class Camera2d{
public:
	Camera2d(int in_width, int in_height);

	std::pair<int, int> getPosition() const;
	void setPosition(int in_x, int in_y);

	int getWidth() const;
	void setWidth(int in_width);
	int getHeight() const;
	void setHeight(int in_height);

	void center(const Rectangle &in_rectangle, std::optional<Rectangle> in_outerBounds);
	const Rectangle& getViewingRectangle() const;

private:
	Rectangle m_viewRect; //<! The viewing rectangle.
};

Rectangle toScreenCoords(const Camera2d &in_camera,
												 const Rectangle &in_rect,
												 uint32_t in_windowId,
												 bool doYFlip = false);

} // CapEngine

#endif // CAPENGINE_CAMERA2D_H

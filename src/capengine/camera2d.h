#ifndef CAPENGINE_CAMERA2D_H
#define CAPENGINE_CAMERA2D_H

#include "collision.h"

#include <cstdint>
#include <optional>

namespace CapEngine
{

class Camera2d
{
   public:
    Camera2d(int in_width, int in_height);

    std::pair<int, int> getPosition() const;
    void setPosition(int in_x, int in_y);

    int getWidth() const;
    void setWidth(int in_width);
    int getHeight() const;
    void setHeight(int in_height);

    void center(const Rectangle &in_rectangle,
                std::optional<Rectangle> in_outerBounds);
    const Rectangle &getViewingRectangle() const;
    float zoom() const;
    void zoom(float in_amount);
    void setZoom(float in_amount);

   private:
    Rectangle m_viewRect;  //<! The viewing rectangle.
    float m_zoom = 1.0;
};

Rectangle toScreenCoords(const Camera2d &in_camera, const Rectangle &in_rect,
						 uint32_t in_windowId, bool doYFlip);

Rectangle toScreenCoords(const Camera2d &in_camera, const Rectangle &in_rect,
                         int windowWidth, int windowHeight, bool doYFlip);

Vector toScreenCoords(const Camera2d &in_camera, const Vector &in_point,
                      uint32_t in_windowId, bool doYFlip);

Vector windowToLocalCoordinate(Camera2d const& in_camera, Vector const& in_point, Rectangle const& in_rectangle, uint32_t windowId, bool doYFlip);

} // namespace CapEngine

#endif // CAPENGINE_CAMERA2D_H

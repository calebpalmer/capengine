#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "matrix.h"
#include "captypes.h"

namespace CapEngine{
  class Viewport
  {
  public:
    Viewport();
    Viewport(int x, int y,
	     int width, int height);
    ~Viewport() = default;

    Rect transformRect(Rect in_rect) const;
    void centerOnObject(Rect objectToCenter,
			      Rect mapDims);
    void setX(int x);
    int getX() const;
    void setY(int y);
    int getY() const;

    Matrix m_transformationMatrix;
    int m_width;
    int m_height;
  };
}


#endif // VIEWPORT_H

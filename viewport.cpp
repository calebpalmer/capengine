#include "viewport.h"

#include "vector.h"

using namespace CapEngine;

Viewport::Viewport() : Viewport(0.0, 0.0, 0.0, 0.0) {}

Viewport::Viewport(int x, int y,
		   int width, int height) :
  m_transformationMatrix(Matrix::createIdentityMatrix()),
  m_width(width), m_height(height)
{
  m_transformationMatrix.getRowVectorRef(0).setD((-1) * x);
  m_transformationMatrix.getRowVectorRef(1).setD(y);
}

int Viewport::getX() const
{
  return (-1) * m_transformationMatrix.getRowVector(0).getD();
}

void Viewport::setX(int x)
{
  m_transformationMatrix.getRowVectorRef(0).setD(-x);
}

int Viewport::getY() const
{
  return m_transformationMatrix.getRowVector(1).getD();
}

void Viewport::setY(int y)
{
  m_transformationMatrix.getRowVectorRef(1).setD(y);
}

Rect Viewport::transformRect(Rect in_rect) const
{
  Vector vector(in_rect.x, in_rect.y, 0.0, 1.0);
  Vector newVector = m_transformationMatrix * vector;

  Rect rect = {static_cast<int>(std::round(newVector.getX())),
	       static_cast<int>(std::round(newVector.getY())),
	       in_rect.w, in_rect.y};
  
  return rect;
}

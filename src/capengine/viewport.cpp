#include "viewport.h"

#include "CapEngineException.h"
#include "collision.h"
#include "vector.h"

using namespace CapEngine;

Viewport::Viewport() : Viewport(0.0, 0.0, 0.0, 0.0) {}

Viewport::Viewport(int x, int y, int width, int height)
	: m_transformationMatrix(Matrix::createIdentityMatrix()), m_width(width),
	  m_height(height)
{
  // TODO commented this out when trying to fix matrix stuff
  // TODO need to figure out if this is needed
  // m_transformationMatrix.getRowVectorRef(0).setD((-1) * x);
  // m_transformationMatrix.getRowVectorRef(1).setD(y);
}

int Viewport::getX() const
{
  return (-1) * m_transformationMatrix.getRowVector(0).getD();
}

void Viewport::setX(int x)
{
  // TODO same as above
  // m_transformationMatrix.getRowVectorRef(0).setD(-x);
}

int Viewport::getY() const
{
  return m_transformationMatrix.getRowVector(1).getD();
}

void Viewport::setY(int y)
{
  // TODO same as above
  // m_transformationMatrix.getRowVectorRef(1).setD(y);
}

//! Perform viewport transformation on point.
/**
 \param points
   \li The point to transform
 \return
   \li The new point
*/
Point Viewport::transform(const Point &point)
{
  auto vector = m_transformationMatrix * Vector(point.x, point.y, 0, 1);
  return {static_cast<int>(vector.getX() + 0.5),
		  static_cast<int>(vector.getY() + 0.5)};
}

Rect Viewport::transformRect(Rect in_rect) const
{
  Vector vector(in_rect.x, in_rect.y, 0, 1);
  Vector newVector = m_transformationMatrix * vector;

  Rect rect = {static_cast<int>(std::round(newVector.getX())),
			   static_cast<int>(std::round(newVector.getY())), in_rect.w,
			   in_rect.h};

  return rect;
}

void Viewport::centerOnObject(Rect objectToCenter, Rect mapDimensions)
{

  Vector objectCenter(objectToCenter.x + (objectToCenter.w / 2),
					  objectToCenter.y + (objectToCenter.h / 2));

  // check horizontal dimension
  int halfWidth = this->m_width / 2;

  if (objectCenter.x - mapDimensions.x > halfWidth &&
	  mapDimensions.w - objectCenter.x > halfWidth) {
	// center the viewport horizontally
	this->setX(objectCenter.x - halfWidth);
  } else if (objectCenter.x - mapDimensions.x <= halfWidth) {
	// horizontally set the viewport all the way to left of the map
	this->setX(mapDimensions.x);
  } else if (mapDimensions.w - objectCenter.x <= halfWidth) {
	// horizontally set the viewport all the way to the right of the map
	this->setX(mapDimensions.w - this->m_width);
  } else {
	throw CapEngineException(
		"Unknown case not being handled when centering viewport horizontally");
  }

  // check vertical
  int halfHeight = this->m_height / 2;

  if (objectCenter.y - mapDimensions.y > halfHeight &&
	  mapDimensions.h - objectCenter.y > halfHeight) {
	// center viewport vertically
	this->setY(objectCenter.y - halfHeight);
  } else if (objectCenter.y - mapDimensions.y <= halfHeight) {
	// move viewport all the way to the top of the map
	this->setY(mapDimensions.y);
  } else if (mapDimensions.h - objectCenter.y <= halfHeight) {
	// move viewport all the way to the bottom of the map
	this->setY(mapDimensions.h - this->m_height);
  } else {
	throw CapEngineException(
		"Unknown case not being handled when centering viewport vertically");
  }
}

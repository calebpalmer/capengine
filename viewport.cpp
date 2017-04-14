#include "viewport.h"

#include "vector.h"
#include "CapEngineException.h"

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
	       in_rect.w, in_rect.h};
  
  return rect;
}

void Viewport::centerOnObject(Rect objectToCenter,
				    Rect mapDimensions) {
  
  Vector objectCenter(objectToCenter.x + (objectToCenter.w / 2),
		      objectToCenter.y + (objectToCenter.h / 2));

  
  // check horizontal dimension
  int halfWidth = this->m_width / 2;

  if(objectCenter.x - mapDimensions.x > halfWidth
     && mapDimensions.w - objectCenter.x > halfWidth){
    // center the viewport horizontally
    this->setX(objectCenter.x - halfWidth);
  }
  else if(objectCenter.x - mapDimensions.x <=  halfWidth){
    // horizontally set the viewport all the way to left of the map
    this->setX(mapDimensions.x);
  }
  else if(mapDimensions.w - objectCenter.x <= halfWidth){
    // horizontally set the viewport all the way to the right of the map
    this->setX(mapDimensions.w - this->m_width);
  }
  else{
    throw CapEngineException("Unknown case not being handled when centering viewport horizontally");
  }

  // check vertical
  int halfHeight = this->m_height / 2;

  if(objectCenter.y - mapDimensions.y > halfHeight
     && mapDimensions.h - objectCenter.y > halfHeight){
    // center viewport vertically
    this->setY(objectCenter.y - halfHeight);
  }
  else if(objectCenter.y - mapDimensions.y <= halfHeight){
    // move viewport all the way to the top of the map
    this->setY(mapDimensions.y);
  }
  else if(mapDimensions.h - objectCenter.y <= halfHeight){
    // move viewport all the way to the bottom of the map
    this->setY(mapDimensions.h - this->m_height);
  }
  else{
    throw CapEngineException("Unknown case not being handled when centering viewport vertically");
  }
  
}


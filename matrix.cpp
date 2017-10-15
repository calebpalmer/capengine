#include "matrix.h"
#include "CapEngineException.h"
#include "trigonometry.h"

#include <cmath>
#include <memory>

using namespace std;

namespace CapEngine {

Matrix::Matrix(CapEngine::Vector vec1, CapEngine::Vector vec2, CapEngine::Vector vec3, CapEngine::Vector vec4){
  vectors.push_back(vec1);
  vectors.push_back(vec2);
  vectors.push_back(vec3);
  vectors.push_back(vec4);
}

std::unique_ptr<float> Matrix::getGLMatrix() const{
  if(vectors.size() != 4){
    throw CapEngineException("Not a 4x4 matrix");
  }
  
  unique_ptr<float> pMatrix(new float[16]);
  
  pMatrix.get()[0] = vectors[0].x;
  pMatrix.get()[1] = vectors[0].y;
  pMatrix.get()[2] = vectors[0].z;
  pMatrix.get()[3] = vectors[0].d;

  pMatrix.get()[4] = vectors[1].x;
  pMatrix.get()[5] = vectors[1].y;
  pMatrix.get()[6] = vectors[1].z;
  pMatrix.get()[7] = vectors[1].d;

  pMatrix.get()[8] = vectors[2].x;
  pMatrix.get()[9] = vectors[2].y;
  pMatrix.get()[10] = vectors[2].z;
  pMatrix.get()[11] = vectors[2].d;

  pMatrix.get()[12] = vectors[3].x;
  pMatrix.get()[13] = vectors[3].y;
  pMatrix.get()[14] = vectors[3].z;
  pMatrix.get()[15] = vectors[3].d;
    
  return std::move(pMatrix);

}

void Matrix::setRowVector(int index, CapEngine::Vector vector)
{
  if(index > 3 || index < 0){
    throw CapEngineException("Matrix indexes must be less than or equal to 3 and greater than or equal to 0");
  }

  vectors[index] = vector;
}

Matrix Matrix::createZeroMatrix()
{
  Vector vec1(0.0, 0.0, 0.0, 0.0);
  Vector vec2(0.0, 0.0, 0.0, 0.0);
  Vector vec3(0.0, 0.0, 0.0, 0.0);
  Vector vec4(0.0, 0.0, 0.0, 0.0);
  Matrix matrix(vec1, vec2, vec3, vec4);
  return matrix;
}

Matrix Matrix::createIdentityMatrix(){
  Vector vec1(1.0, 0.0, 0.0, 0.0);
  Vector vec2(0.0, 1.0, 0.0, 0.0);
  Vector vec3(0.0, 0.0, 1.0, 0.0);
  Vector vec4(0.0, 0.0, 0.0, 1.0);
  Matrix matrix(vec1, vec2, vec3, vec4);
  return matrix;
}

Matrix Matrix::createTranslationMatrix(real x, real y, real z){
  Vector vec1(0.0, 0.0, 0.0, x);
  Vector vec2(0.0, 0.0, 0.0, y);
  Vector vec3(0.0, 0.0, 0.0, z);
  Vector vec4(0.0, 0.0, 0.0, 1.0);
  Matrix matrix(vec1, vec2, vec3, vec4);
  return matrix;
}

Matrix Matrix::createScaleMatrix(real sx, real sy, real sz){
  Vector vec1(sx, 0.0, 0.0, 0.0);
  Vector vec2(0.0, sy, 0.0, 0.0);
  Vector vec3(0.0, 0.0, sz, 0.0);
  Vector vec4(0.0, 0.0, 0.0, 1.0);
  Matrix matrix(vec1, vec2, vec3, vec4);
  return matrix;
}

Matrix Matrix::createXRotationMatrix(real degrees){
  real radAngle = DEGTORAD(degrees);
  real cosAngle = cos(radAngle);
  real sinAngle = sin(radAngle);

  Vector vec1(1.0, 0.0, 0.0, 0.0);
  Vector vec2(0.0, cosAngle, -sinAngle, 0.0);
  Vector vec3(0.0, sinAngle, cosAngle, 0.0);
  Vector vec4(0.0, 0.0, 0.0, 1.0);
  Matrix matrix(vec1, vec2, vec3, vec4);
  return matrix;
}

Matrix Matrix::createYRotationMatrix(real degrees){
  real radAngle = DEGTORAD(degrees);
  real cosAngle = cos(radAngle);
  real sinAngle = sin(radAngle);

  Vector vec1(cosAngle, 0.0, sinAngle, 0.0);
  Vector vec2(0.0, 1.0, 0.0, 0.0);
  Vector vec3(-sinAngle, 0.0, cosAngle, 0.0);
  Vector vec4(0.0, 0.0, 0.0, 1.0);
  Matrix  matrix(vec1, vec2, vec3, vec4);
  return matrix;
}

Matrix Matrix::createZRotationMatrix(real degrees){
  real radAngle = DEGTORAD(degrees);
  real cosAngle = cos(radAngle);
  real sinAngle = sin(radAngle);

  Vector vec1(cosAngle, sinAngle, sinAngle,0.0);
  Vector vec2(-sinAngle, cosAngle, 0.0, 0.0);
  Vector vec3(0.0, 0.0, 1.0, 0.0);
  Vector vec4(0.0, 0.0, 0.0, 1.0);
  Matrix matrix(vec1, vec2, vec3, vec4);
  return matrix;
}

const MatrixContainer& Matrix::getVectors() const{
  return vectors;
}

Matrix Matrix::operator*(const Matrix& right) const{
  real cell1, cell2, cell3, cell4;
  // column 1
  // multiply the first row by the first column vector (column major matrix)
  cell1 = (vectors[0].getX() * right.getVectors()[0].getX()) + (vectors[1].getX() * right.getVectors()[0].getY()) 
    + (vectors[2].getX() * right.getVectors()[0].getZ()) + (vectors[3].getX() * right.getVectors()[0].getD());
					       
  cell2 = (vectors[0].getY() * right.getVectors()[0].getX()) + (vectors[1].getY() * right.getVectors()[0].getY()) 
    + (vectors[2].getY() * right.getVectors()[0].getZ()) + (vectors[3].getY() * right.getVectors()[0].getD());

  cell3 = (vectors[0].getZ() * right.getVectors()[0].getX()) + (vectors[1].getZ() * right.getVectors()[0].getY()) 
    + (vectors[2].getZ() * right.getVectors()[0].getZ()) + (vectors[3].getZ() * right.getVectors()[0].getD());

  cell4 = (vectors[0].getD() * right.getVectors()[0].getX()) + (vectors[1].getD() * right.getVectors()[0].getY()) 
    + (vectors[2].getD() * right.getVectors()[0].getZ()) + (vectors[3].getD() * right.getVectors()[0].getD());

  Vector column1(cell1, cell2, cell3, cell4);

  // column 2
    cell1 = (vectors[0].getX() * right.getVectors()[1].getX()) + (vectors[1].getX() * right.getVectors()[1].getY()) 
    + (vectors[2].getX() * right.getVectors()[1].getZ()) + (vectors[3].getX() * right.getVectors()[1].getD());
					       
  cell2 = (vectors[0].getY() * right.getVectors()[1].getX()) + (vectors[1].getY() * right.getVectors()[1].getY()) 
    + (vectors[2].getY() * right.getVectors()[1].getZ()) + (vectors[3].getY() * right.getVectors()[1].getD());

  cell3 = (vectors[0].getZ() * right.getVectors()[1].getX()) + (vectors[1].getZ() * right.getVectors()[1].getY()) 
    + (vectors[2].getZ() * right.getVectors()[1].getZ()) + (vectors[3].getZ() * right.getVectors()[1].getD());

  cell4 = (vectors[0].getD() * right.getVectors()[1].getX()) + (vectors[1].getD() * right.getVectors()[1].getY()) 
    + (vectors[2].getD() * right.getVectors()[1].getZ()) + (vectors[3].getD() * right.getVectors()[1].getD());

  Vector column2(cell1, cell2, cell3, cell4);

  // column 3
  cell1 = (vectors[0].getX() * right.getVectors()[2].getX()) + (vectors[1].getX() * right.getVectors()[2].getY()) 
    + (vectors[2].getX() * right.getVectors()[2].getZ()) + (vectors[3].getX() * right.getVectors()[2].getD());
					       
  cell2 = (vectors[0].getY() * right.getVectors()[2].getX()) + (vectors[1].getY() * right.getVectors()[2].getY()) 
    + (vectors[2].getY() * right.getVectors()[2].getZ()) + (vectors[3].getY() * right.getVectors()[2].getD());

  cell3 = (vectors[0].getZ() * right.getVectors()[2].getX()) + (vectors[1].getZ() * right.getVectors()[2].getY()) 
    + (vectors[2].getZ() * right.getVectors()[2].getZ()) + (vectors[3].getZ() * right.getVectors()[2].getD());

  cell4 = (vectors[0].getD() * right.getVectors()[2].getX()) + (vectors[1].getD() * right.getVectors()[2].getY()) 
    + (vectors[2].getD() * right.getVectors()[2].getZ()) + (vectors[3].getD() * right.getVectors()[2].getD());

  Vector column3(cell1, cell2, cell3, cell4);

  // column 4
  cell1 = (vectors[0].getX() * right.getVectors()[3].getX()) + (vectors[1].getX() * right.getVectors()[3].getY()) 
    + (vectors[2].getX() * right.getVectors()[3].getZ()) + (vectors[3].getX() * right.getVectors()[3].getD());
					       
  cell2 = (vectors[0].getY() * right.getVectors()[3].getX()) + (vectors[1].getY() * right.getVectors()[3].getY()) 
    + (vectors[2].getY() * right.getVectors()[3].getZ()) + (vectors[3].getY() * right.getVectors()[3].getD());

  cell3 = (vectors[0].getZ() * right.getVectors()[3].getX()) + (vectors[1].getZ() * right.getVectors()[3].getY()) 
    + (vectors[2].getZ() * right.getVectors()[3].getZ()) + (vectors[3].getZ() * right.getVectors()[3].getD());

  cell4 = (vectors[0].getD() * right.getVectors()[3].getX()) + (vectors[1].getD() * right.getVectors()[3].getY()) 
    + (vectors[2].getD() * right.getVectors()[3].getZ()) + (vectors[3].getD() * right.getVectors()[3].getD());

  Vector column4(cell1, cell2, cell3, cell4);

  Matrix newMatrix(column1, column2, column3, column4);
  return newMatrix;
}

Vector Matrix::operator*(const Vector& in_right) const{
  
  Vector right(in_right);
  
  Vector xTransform = this->vectors[0];
  real x =
    right.getX() * xTransform.getX() +
    right.getY() * xTransform.getY() +
    right.getZ() * xTransform.getZ() +
    right.getD() * xTransform.getD();
  
  Vector yTransform = this->vectors[1];
  real y =
    right.getX() * yTransform.getX() +
    right.getY() * yTransform.getY() +
    right.getZ() * yTransform.getZ() +
    right.getD() * yTransform.getD();
    
  Vector zTransform = this->vectors[2];
  real z =
    right.getX() * zTransform.getX() +
    right.getY() * zTransform.getY() +
    right.getZ() * zTransform.getZ() +
    right.getD() * zTransform.getD();
  
  Vector dTransform = this->vectors[3];
  real d =
    right.getX() * dTransform.getX() +
    right.getY() * dTransform.getY() +
    right.getZ() * dTransform.getZ() +
    right.getD() * dTransform.getD();

  return Vector(x, y, z, d);
}

Matrix Matrix::operator+(const Matrix& right) const{
  auto leftVec1 = this->getRowVector(0);
  auto leftVec2 = this->getRowVector(1);
  auto leftVec3 = this->getRowVector(2);
  auto leftVec4 = this->getRowVector(3);

  auto rightVec1 = right.getRowVector(0);
  auto rightVec2 = right.getRowVector(1);
  auto rightVec3 = right.getRowVector(2);
  auto rightVec4 = right.getRowVector(3);

  return Matrix(leftVec1 + rightVec1,
		leftVec2 + rightVec2,
		leftVec3 + rightVec3,
		leftVec4 + rightVec4);
}

Matrix Matrix::operator-(const Matrix& right) const {
  auto leftVec1 = this->getRowVector(0);
  auto leftVec2 = this->getRowVector(1);
  auto leftVec3 = this->getRowVector(2);
  auto leftVec4 = this->getRowVector(3);

  auto rightVec1 = right.getRowVector(0);
  auto rightVec2 = right.getRowVector(1);
  auto rightVec3 = right.getRowVector(2);
  auto rightVec4 = right.getRowVector(3);

  return Matrix(leftVec1 - rightVec1,
		leftVec2 - rightVec2,
		leftVec3 - rightVec3,
		leftVec4 - rightVec4);
}

Vector Matrix::getColumnVector(int index) const{
  if(index > 3 || index < 0){
    throw CapEngineException("Matrix indexes must be less than or equal to 3 and greater than or equal to 0");
  }

  int x, y, z, d = 0;
  switch(index){
  case 0:
    x = vectors[0].x;
    y = vectors[1].x;
    z = vectors[2].x;
    d = vectors[3].x;
    break;
  case 1:
    x = vectors[0].y;
    y = vectors[1].y;
    z = vectors[2].y;
    d = vectors[3].y;
    break;
  case 2:
    x = vectors[0].z;
    y = vectors[1].z;
    z = vectors[2].z;
    d = vectors[3].z;
    break;
  case 3:
    x = vectors[0].d;
    y = vectors[1].d;
    z = vectors[2].d;
    d = vectors[3].d;
    break;
  default:
    throw CapEngineException("Invalid index");
    break;
  }
  Vector retVector(x, y, z, d);
  return retVector;
}

Vector Matrix::getRowVector(int index) const{
  if(index > 3 || index < 0){
    throw CapEngineException("Matrix indexes must be less than or equal to 3 and greater than or equal to 0");
  }
  
  Vector retVector(vectors[index]);
  return retVector;
}

Vector& Matrix::getRowVectorRef(int index){
  if(index > 3 || index < 0){
    throw CapEngineException("Matrix indexes must be less than or equal to 3 and greater than or equal to 0");
  }
  
  return vectors[index];
}

std::ostream& operator<<(std::ostream& stream, const CapEngine::Matrix& matrix){

  stream << "Matrix(";
  for(size_t i = 0; i < 4; i++){
    auto && vector = matrix.getRowVector(i);
    stream << "(" << vector.getX() << ", " << vector.getY()
	   << ", " << vector.getZ() << ", " << vector.getD() << ")";
  }
  stream << ")";
  return stream;
}

}

#include "matrix.h"
#include "CapEngineException.h"
#include "trigonometry.h"

#include <cmath>
#include <memory>

using namespace std;

namespace CapEngine
{

//! Create a Matrix
/**

 Each Vector passed represents a column in the matrix.  eg:

 Matrix({1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {2, 3, 0, 1})
 actually looks like this:

 [ 1 0 0 2 ]
 [ 0 1 0 3 ]
 [ 0 0 1 0 ]
 [ 0 0 0 1 ]

 \param vec1
 \param vec2
 \param vec3
 \parem vec4
 \return
*/
Matrix::Matrix(CapEngine::Vector vec1, CapEngine::Vector vec2,
               CapEngine::Vector vec3, CapEngine::Vector vec4)
{
  vectors.push_back(vec1);
  vectors.push_back(vec2);
  vectors.push_back(vec3);
  vectors.push_back(vec4);
}

std::unique_ptr<float> Matrix::getGLMatrix() const
{
  if (vectors.size() != 4) {
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

  return pMatrix;
}

void Matrix::setRowVector(int index, CapEngine::Vector vector)
{
  if (index > 3 || index < 0) {
    throw CapEngineException("Matrix indexes must be less than or equal to 3 "
                             "and greater than or equal to 0");
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

Matrix Matrix::createIdentityMatrix()
{
  Vector vec1(1.0, 0.0, 0.0, 0.0);
  Vector vec2(0.0, 1.0, 0.0, 0.0);
  Vector vec3(0.0, 0.0, 1.0, 0.0);
  Vector vec4(0.0, 0.0, 0.0, 1.0);
  Matrix matrix(vec1, vec2, vec3, vec4);
  return matrix;
}

Matrix Matrix::createTranslationMatrix(real x, real y, real z)
{
  Vector vec1(1.0, 0.0, 0.0, 0.0);
  Vector vec2(0.0, 1.0, 0.0, 0.0);
  Vector vec3(0.0, 0.0, 1.0, 0.0);
  Vector vec4(x, y, z, 1.0);
  Matrix matrix(vec1, vec2, vec3, vec4);
  return matrix;
}

Matrix Matrix::createScaleMatrix(real sx, real sy, real sz)
{
  Vector vec1(sx, 0.0, 0.0, 0.0);
  Vector vec2(0.0, sy, 0.0, 0.0);
  Vector vec3(0.0, 0.0, sz, 0.0);
  Vector vec4(0.0, 0.0, 0.0, 1.0);
  Matrix matrix(vec1, vec2, vec3, vec4);
  return matrix;
}

Matrix Matrix::createXRotationMatrix(real degrees)
{
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

Matrix Matrix::createYRotationMatrix(real degrees)
{
  real radAngle = DEGTORAD(degrees);
  real cosAngle = cos(radAngle);
  real sinAngle = sin(radAngle);

  Vector vec1(cosAngle, 0.0, sinAngle, 0.0);
  Vector vec2(0.0, 1.0, 0.0, 0.0);
  Vector vec3(-sinAngle, 0.0, cosAngle, 0.0);
  Vector vec4(0.0, 0.0, 0.0, 1.0);
  Matrix matrix(vec1, vec2, vec3, vec4);
  return matrix;
}

Matrix Matrix::createZRotationMatrix(real degrees)
{
  real radAngle = DEGTORAD(degrees);
  real cosAngle = cos(radAngle);
  real sinAngle = sin(radAngle);

  Vector vec1(cosAngle, sinAngle, sinAngle, 0.0);
  Vector vec2(-sinAngle, cosAngle, 0.0, 0.0);
  Vector vec3(0.0, 0.0, 1.0, 0.0);
  Vector vec4(0.0, 0.0, 0.0, 1.0);
  Matrix matrix(vec1, vec2, vec3, vec4);
  return matrix;
}

const MatrixContainer &Matrix::getVectors() const { return vectors; }

//! matrix multiplication
/**
 \param other - The other matrix to multipy
 \return - The new matrix
*/
Matrix Matrix::operator*(const Matrix &other) const
{
  assert(this->vectors.size() == 4);
  assert(other.vectors.size() == 4);

  // column vector 1
  Vector v1(dotProduct(this->getRowVector(0), other.getColumnVector(0)),
            dotProduct(this->getRowVector(1), other.getColumnVector(0)),
            dotProduct(this->getRowVector(2), other.getColumnVector(0)),
            dotProduct(this->getRowVector(3), other.getColumnVector(0)));

  // column vector 2
  Vector v2(dotProduct(this->getRowVector(0), other.getColumnVector(1)),
            dotProduct(this->getRowVector(1), other.getColumnVector(1)),
            dotProduct(this->getRowVector(2), other.getColumnVector(1)),
            dotProduct(this->getRowVector(3), other.getColumnVector(1)));

  // column vector 3
  Vector v3(dotProduct(this->getRowVector(0), other.getColumnVector(2)),
            dotProduct(this->getRowVector(1), other.getColumnVector(2)),
            dotProduct(this->getRowVector(2), other.getColumnVector(2)),
            dotProduct(this->getRowVector(3), other.getColumnVector(2)));

  // column vector 4
  Vector v4(dotProduct(this->getRowVector(0), other.getColumnVector(3)),
            dotProduct(this->getRowVector(1), other.getColumnVector(3)),
            dotProduct(this->getRowVector(2), other.getColumnVector(3)),
            dotProduct(this->getRowVector(3), other.getColumnVector(3)));

  return Matrix(v1, v2, v3, v4);
}

//! Multiply a vector by a matrix
/**

 The vector is on the right hand side so it is
 a column vector.

 \param v - The Vector to multiply.
 \return - The transformed vector.
*/
Vector Matrix::operator*(const Vector &v) const
{

  return Vector(dotProduct(this->getRowVector(0), v),
                dotProduct(this->getRowVector(1), v),
                dotProduct(this->getRowVector(2), v),
                dotProduct(this->getRowVector(3), v));
}

Matrix Matrix::operator+(const Matrix &right) const
{
  auto leftVec1 = this->getRowVector(0);
  auto leftVec2 = this->getRowVector(1);
  auto leftVec3 = this->getRowVector(2);
  auto leftVec4 = this->getRowVector(3);

  auto rightVec1 = right.getRowVector(0);
  auto rightVec2 = right.getRowVector(1);
  auto rightVec3 = right.getRowVector(2);
  auto rightVec4 = right.getRowVector(3);

  return Matrix(leftVec1 + rightVec1, leftVec2 + rightVec2,
                leftVec3 + rightVec3, leftVec4 + rightVec4);
}

Matrix Matrix::operator-(const Matrix &right) const
{
  auto leftVec1 = this->getRowVector(0);
  auto leftVec2 = this->getRowVector(1);
  auto leftVec3 = this->getRowVector(2);
  auto leftVec4 = this->getRowVector(3);

  auto rightVec1 = right.getRowVector(0);
  auto rightVec2 = right.getRowVector(1);
  auto rightVec3 = right.getRowVector(2);
  auto rightVec4 = right.getRowVector(3);

  return Matrix(leftVec1 - rightVec1, leftVec2 - rightVec2,
                leftVec3 - rightVec3, leftVec4 - rightVec4);
}

Vector Matrix::getRowVector(int index) const
{
  if (index > 3 || index < 0) {
    throw CapEngineException("Matrix indexes must be less than or equal to 3 "
                             "and greater than or equal to 0");
  }

  real x, y, z, d = 0.0;
  switch (index) {
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

Vector Matrix::getColumnVector(int index) const
{
  if (index > 3 || index < 0) {
    throw CapEngineException("Matrix indexes must be less than or equal to 3 "
                             "and greater than or equal to 0");
  }

  Vector retVector(vectors[index]);
  return retVector;
}

Vector &Matrix::getColumnVectorRef(int index)
{
  if (index > 3 || index < 0) {
    throw CapEngineException("Matrix indexes must be less than or equal to 3 "
                             "and greater than or equal to 0");
  }

  return vectors[index];
}

std::ostream &operator<<(std::ostream &stream, const CapEngine::Matrix &matrix)
{

  stream << "Matrix(";
  for (size_t i = 0; i < 4; i++) {
    auto &&vector = matrix.getRowVector(i);
    stream << "(" << vector.getX() << ", " << vector.getY() << ", "
           << vector.getZ() << ", " << vector.getD() << ")";
  }
  stream << ")";
  return stream;
}

//! Returns a string representation
/**
 \return - The string representation
*/
std::string Matrix::toString(bool pretty) const
{
  std::stringstream stream;
  for (size_t i = 0; i < 4; i++) {
    auto &&vector = this->getRowVector(i);
    stream << "(" << vector.getX() << ", " << vector.getY() << ", "
           << vector.getZ() << ", " << vector.getD() << ")";
    if (pretty) {
      stream << std::endl;
    }
  }
  stream << ")";
  return stream.str();
}

//! operator==
bool Matrix::operator==(const Matrix &other) const
{
  bool equal = true;
  if (this->vectors.size() != other.vectors.size())
    return false;

  for (size_t i = 0; i < vectors.size(); i++) {
    if (this->vectors[i] != other.vectors[i])
      return false;
  }

  return true;
}

} // namespace CapEngine

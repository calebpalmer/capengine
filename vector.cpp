//@brief - implementation file for the Vector class
//@author - Caleb Palmer

#include <cstddef>
#include <cmath>
#include "vector.h"
#include "trigonometry.h"

using namespace CapEngine;
using namespace std;

//default constructor for a Vector.  Sets everything to zeros.
Vector::Vector() :
  x(0.0), y(0.0), z(0.0), d(0.0) { }

/* constructor for Vector
   \param x_in the x component of the vector
   \param y_in the y component of the vector
   \param slopeX_in - the slope for the x component of the vector
   \param slopeY_in - the slope for the y component of the vector
*/
Vector::Vector(real x_in, real y_in, real z_in, real d_in) : 
  x(x_in), y(y_in), z(z_in), d(d_in) { }

// copy constructor
Vector::Vector(const Vector& vec){
  this->x = vec.x;
  this->y = vec.y;
  this->z = vec.z;
  this->d = vec.d;
}

//constructor taking a PolarVector as input, creates a vector on the xy plane
Vector::Vector(const PolarVector& pVecIn){
  x = pVecIn.mag * cos(DEGTORAD(pVecIn.deg));
  y = pVecIn.mag * sin(DEGTORAD(pVecIn.deg));
  z = 0.0;
  d = 0.0;
}

Vector::~Vector(){ }


/** returns x component of vector
    \returns the x component of the vector
*/
real Vector::getX() const {
  return x;
}

/** returns y component of vector
    \returns the y component of the vector
*/
real Vector::getY() const {
  return y;
}

/** returns z component of vector
    \returns the z component of the vector
*/
real Vector::getZ() const {
  return z;
}

/** returns d component of vector
    \returns the d component of the vector
*/
real Vector::getD() const {
  return d;
}

/** set the x componnet
    \param the x component
*/
void Vector::setX(real xIn) {
  x = xIn;
}

/** set the y componnet
    \param the y component
*/
void Vector::setY(real yIn) {
  y = yIn;
}

/** set the y componnet
    \param the y component
*/
void Vector::setZ(real zIn) {
  z = zIn;
}

/** set the d componnet
    \param the d component
*/
void Vector::setD(real dIn) {
  d = dIn;
}

/*
  \fn operator+ definition
  \param vec1
  \param vec2
  \return Vector
 */
Vector Vector::operator+(const Vector& vec2) const{
  return Vector(x + vec2.getX(), y + vec2.getY(), z + vec2.getZ());
}

/*
  \fn operator- definition
  \param vec1
  \param vec2
  \return Vector
 */
Vector Vector::operator-(const Vector& vec2) const{
  return Vector(x - vec2.getX(), y - vec2.getY(), z - vec2.getZ());
}

// TODO: fix this so that it returns a proper value
/*
  \fn operator= definition
  \param vec1
  \param vec2
  \return Vector
 */
Vector& Vector::operator=(const Vector& vec){
  Vector newVector;
  x = vec.x;
  y = vec.y;
  z = vec.z;
  d = vec.d;
  return *this;
}


/*
  \fn calculate the distance from the given Vector
  \param vec
  \li the vector to calculate the distance from
  \return
  \li the distance of this from vec.
 */
real CapEngine::distance(const Vector& vec1, const Vector& vec2){
  // sqrt (x*x + y*y
  real xdelta = vec1.getX() - vec2.getX();
  real ydelta = vec1.getY() - vec2.getY();
  real zdelta = vec1.getZ() - vec2.getZ();
  return sqrt(xdelta * xdelta + ydelta * ydelta + zdelta * zdelta);
}

/*
  \fn multiply the members of the vector by given factor
  \param factor
  \li the amount to scale the vector components
 */
void Vector::scale(real factor){
  x = x * factor;
  y = y * factor;
  z = z * factor;
}

/*
  \fn calculate the magnitude of the vector
  \return
  \li the magnitude of the vector
 */
real Vector::magnitude() const{
  return distance(*this, Vector(0,0,0));
}

/*
  \fn return a normalized version of this vector
  \return 
  \li ptr to a normalized Vector
 */
Vector& Vector::normalize() const{
  real mag = this->magnitude();
  unique_ptr<Vector> normalVec(new Vector(x/mag, y/mag, z/mag));
  return *(normalVec.release());
}

/*
  \fn calculate the dot product of this and passed vector
Generally tells how similar two vectors are
if > 0 -> theta between 0 and 90
if == 0 -> theta is 90
if < 0 -> theta between 90 and 180
  \param vec
  \return 
  \li the dot product
 */
real CapEngine::dotProduct(const Vector& vec1, const Vector& vec2){
    return (vec1.getX() * vec2.getX() + vec1.getY() * vec2.getY() + vec1.getZ() * vec2.getZ());
  }

/*
  \fn calculate the crossProduct of this vector and passed vector
  \param vec
  \return
  \li the cross product of the two vectors
*/
Vector& CapEngine::crossProduct(const Vector& vec1, const Vector& vec2){
    real newx = vec1.getY()*vec2.getZ() + vec1.getZ()*vec2.getY();
    real newy = vec1.getZ()*vec2.getX() + vec1.getX()*vec2.getZ();
    real newz = vec1.getX()*vec2.getY() + vec1.getY()*vec2.getX();
  
    unique_ptr<Vector> retVal(new Vector(newx, newy, newz));
    return *(retVal.release());
}

/*
  \fn calculate the projected vector of this onto vec
  \param vec the vector to be projected onto
  \return
  \li the project vector of this onto vec
 */
Vector& CapEngine::projectedVector(const Vector& vec1, const Vector& vec2){
  real dotProd = CapEngine::dotProduct(vec1, vec2);
  real magVec = vec2.magnitude();
  unique_ptr<Vector> retVal(new Vector(vec2));
  retVal->scale( dotProd / (magVec * magVec) );
  return *(retVal.release());
}

/*
  \fn calculate the perpendicular of the projected vector of this onto vec
  \param vec the vector to be projected onto
  \return
  \li the project vector of this onto vec
 */
Vector& projectedPerpendicularVector(const Vector& vec1, const Vector& vec2){
  unique_ptr<Vector> pVector(&(CapEngine::projectedVector(vec1, vec2)));
  unique_ptr<Vector> retVal(new Vector(vec2 - *pVector));
  return *(retVal.release());
}

/*
  \fn calculate the angle between this vector and the given vector along xy axes
  \param vec
  \return
  \li the angle
 */
real angle(const Vector& vec1, const Vector& vec2){
  // acos (vec.x - x / distance)
  real distance = CapEngine::distance(vec1, vec2);
  real xDelta = vec1.getX() - vec2.getX();
  real radAngle = acos(distance / xDelta);
  return RADTODEG(radAngle);
}

//! Not implemented for 2d vectors.  Throws exception.
/*!
 */
Vector& surfaceNormal(const Vector& vec1, const Vector& vec2){
  Vector& temp = CapEngine::crossProduct(vec1, vec2);
  Vector& retVal = temp.normalize();
  return retVal;
}

//! return a PolarVector version of current vector
/*!
  \return
  \li pointer to a PolarVector.  Ownership is passed.
 */
PolarVector* Vector::toPolarVector() const{
  return nullptr;
}

#ifndef VECTOR_H
#define VECTOR_H

// \brief - the header file for the Vector class
// \author - Caleb Palmer

//TODO change Vector& return values to Vector

#include <memory>

#include "captypes.h"

namespace CapEngine{

  struct PolarVector{
    real mag;
    real deg;
  };
  
  class Vector{

  public:
    Vector();
    Vector(real x_in, real y_in=0.0, real z_in=0.0, real d_in=0.0);
    Vector(const PolarVector& pVecIn);
    Vector(const Vector&);
    ~Vector();
    real getX() const;    
    real getY() const;
    real getZ() const;
    real getD() const;
    void setX(real xIn);
    void setY(real yIn);
    void setZ(real zIn);
    void setD(real dIn);
    Vector& operator=(const Vector& vec);  //TODO
    bool operator==(const Vector& vec) const; //TODO
    bool operator!=(const Vector& vec) const; //TODO
    Vector operator/(const Vector& vec) const;//TODO
    Vector operator+(const Vector& vec) const;
    Vector operator-(const Vector& vec) const;
    Vector operator/(real scalar) const;//TODO
    Vector operator*(real scalar) const;//TODO
    Vector& operator+=(const Vector& vec);//TODO
    Vector& operator-=(const Vector& vec);//TODO
    Vector& operator*=(real scalar);//TODO
    Vector& operator/=(real scalar);//TODO
    real operator*(const Vector& vec) const;  //TODO Dot Product
    void scale(real factor);
    real magnitude() const;
    Vector& normalize() const;
    PolarVector* toPolarVector() const;
    
    real x;
    real y;
    real z;
    real d;

  };

  Vector& crossProduct(const Vector& vec1, const Vector& vec2);
  real dotProduct(const Vector& vec1, const Vector& vec2);
  Vector& projectedVector(const Vector& vec1, const Vector& vec2); 
  Vector& projectedPerpendicularVector(const Vector& vec1, const Vector& vec2); 
  Vector& surfaceNormal(const Vector& vec1, const Vector& vec2);
  real distance(const Vector& vec1, const Vector& vec2);
  real angle(const Vector& vec1, const Vector& vec2);
}
#endif

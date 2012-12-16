#ifndef VECTOR_H
#define VECTOR_H

// \brief - the header file for the Vector class
// \author - Caleb Palmer

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
    Vector operator+(const Vector& vec);
    Vector operator-(const Vector& vec);
    real distance(const Vector& vec) const;
    void scale(real factor);
    real magnitude() const;
    Vector* normalize() const;
    real dotProduct(const Vector& vec) const;
    Vector* crossProduct(const Vector& vec) const;
    Vector* projectedVector(const Vector& vec) const; //TODO
    real angle(const Vector& vec) const;
    Vector* surfaceNormal(const Vector& vec) const;
    PolarVector* toPolarVector() const;

  private:
    
    real x;
    real y;
    real z;
    real d;
    
  private:
    Vector& operator=(const Vector&){return *this;}
    

  };
}
#endif

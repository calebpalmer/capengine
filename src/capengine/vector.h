#ifndef VECTOR_H
#define VECTOR_H

// \brief - the header file for the Vector class
// \author - Caleb Palmer

// TODO change Vector& return values to Vector

#include <memory>

#include "captypes.h"

namespace CapEngine
{

struct PolarVector {
	real mag;
	real deg;
};

class Vector
{
   public:
	Vector();
	explicit Vector(real x_in, real y_in = 0.0, real z_in = 0.0, real d_in = 1.0);
	explicit Vector(int x_in, int y_in = 0, int z_in = 0, int d_in = 1);
	explicit Vector(const PolarVector &pVecIn);
	Vector(const Vector &);
	~Vector();
	real getX() const;
	real getY() const;
	real getZ() const;
	real getD() const;
	void setX(real xIn);
	void setY(real yIn);
	void setZ(real zIn);
	void setD(real dIn);
	Vector &operator=(const Vector &vec);       // TODO
	bool operator==(const Vector &vec) const;   // TODO
	bool operator!=(const Vector &vec) const;   // TODO
	Vector operator/(const Vector &vec) const;  // TODO
	Vector operator+(const Vector &vec) const;
	Vector operator-(const Vector &vec) const;
	Vector operator/(real scalar) const;      // TODO
	Vector operator*(real scalar) const;      // TODO
	Vector &operator+=(const Vector &vec);    // TODO
	Vector &operator-=(const Vector &vec);    // TODO
	Vector &operator*=(real scalar);          // TODO
	Vector &operator/=(real scalar);          // TODO
	real operator*(const Vector &vec) const;  // TODO Dot Product
	void scale(real factor);
	real magnitude() const;
	Vector normalize() const;
	Vector absolute() const;
	real slope() const;
	real angle2d() const;
	PolarVector *toPolarVector() const;
	std::string toString() const;
	friend std::ostream &operator<<(std::ostream &stream, const Vector &vector);

	real x;
	real y;
	real z;
	real d;
};

Vector crossProduct(const Vector &vec1, const Vector &vec2);
real dotProduct(const Vector &vec1, const Vector &vec2);
Vector projectedVector(const Vector &vec1, const Vector &vec2);
Vector projectedPerpendicularVector(const Vector &vec1, const Vector &vec2);
Vector surfaceNormal(const Vector &vec1, const Vector &vec2);
real distance(const Vector &vec1, const Vector &vec2);
real angle(const Vector &vec1, const Vector &vec2);

} // namespace CapEngine
#endif

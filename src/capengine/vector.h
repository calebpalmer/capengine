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
	double mag;
	double deg;
};

class Vector
{
   public:
	Vector();
	explicit Vector(double x_in, double y_in = 0.0, double z_in = 0.0, double d_in = 1.0);
	explicit Vector(int x_in, int y_in = 0, int z_in = 0, int d_in = 1);
	explicit Vector(const PolarVector &pVecIn);
	Vector(const Vector &);
	~Vector();
	double getX() const;
	double getY() const;
	double getZ() const;
	double getD() const;
	void setX(double xIn);
	void setY(double yIn);
	void setZ(double zIn);
	void setD(double dIn);
	Vector &operator=(const Vector &vec);       // TODO
	bool operator==(const Vector &vec) const;   // TODO
	bool operator!=(const Vector &vec) const;   // TODO
	Vector operator/(const Vector &vec) const;  // TODO
	Vector operator+(const Vector &vec) const;
	Vector operator-(const Vector &vec) const;
	Vector operator/(double scalar) const;      // TODO
	Vector operator*(double scalar) const;      // TODO
	Vector &operator+=(const Vector &vec);    // TODO
	Vector &operator-=(const Vector &vec);    // TODO
	Vector &operator*=(double scalar);          // TODO
	Vector &operator/=(double scalar);          // TODO
	double operator*(const Vector &vec) const;  // TODO Dot Product
	void scale(double factor);
	double magnitude() const;
	Vector normalize() const;
	Vector absolute() const;
	double slope() const;
	double angle2d() const;
	PolarVector *toPolarVector() const;
	std::string toString() const;
	friend std::ostream &operator<<(std::ostream &stream, const Vector &vector);

	double x;
	double y;
	double z;
	double d;
};

Vector crossProduct(const Vector &vec1, const Vector &vec2);
double dotProduct(const Vector &vec1, const Vector &vec2);
Vector projectedVector(const Vector &vec1, const Vector &vec2);
Vector projectedPerpendicularVector(const Vector &vec1, const Vector &vec2);
Vector surfaceNormal(const Vector &vec1, const Vector &vec2);
double distance(const Vector &vec1, const Vector &vec2);
double angle(const Vector &vec1, const Vector &vec2);

} // namespace CapEngine
#endif

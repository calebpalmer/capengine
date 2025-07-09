#ifndef VECTOR_H
#define VECTOR_H

// \brief - the header file for the Vector class
// \author - Caleb Palmer

#include <string>

namespace CapEngine {

struct PolarVector {
    double mag;
    double deg;
};

class Vector {
   public:
    Vector() = default;
    explicit Vector(double x_in, double y_in = 0.0, double z_in = 0.0, double d_in = 1.0);
    explicit Vector(int x_in, int y_in = 0, int z_in = 0, int d_in = 1);
    explicit Vector(const PolarVector& pVecIn);

    ~Vector() = default;
    Vector(const Vector&) = default;
    Vector& operator=(const Vector&) = default;
    Vector(Vector&&) noexcept = default;
    Vector& operator=(Vector&&) noexcept = default;

    [[nodiscard]] double getX() const;
    [[nodiscard]] double getY() const;
    [[nodiscard]] double getZ() const;
    [[nodiscard]] double getD() const;
    void setX(double xIn);
    void setY(double yIn);
    void setZ(double zIn);
    void setD(double dIn);

    bool operator==(const Vector& vec) const;
    bool operator!=(const Vector& vec) const;
    Vector operator/(const Vector& vec) const;
    Vector operator+(const Vector& vec) const;
    Vector operator-(const Vector& vec) const;
    Vector operator/(double scalar) const;
    Vector operator*(double scalar) const;
    Vector& operator+=(const Vector& vec);
    Vector& operator-=(const Vector& vec);
    Vector& operator*=(double scalar);
    Vector& operator/=(double scalar);
    double operator*(const Vector& vec) const;

    void scale(double factor);
    [[nodiscard]] double magnitude() const;
    [[nodiscard]] Vector normalize() const;
    [[nodiscard]] Vector absolute() const;
    [[nodiscard]] double slope() const;
    [[nodiscard]] double angle2d() const;
    [[nodiscard]] std::string toString() const;
    friend std::ostream& operator<<(std::ostream& stream, const Vector& vector);

    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
    double d = 0.0;
};

Vector crossProduct(const Vector& vec1, const Vector& vec2);
double dotProduct(const Vector& vec1, const Vector& vec2);
Vector projectedVector(const Vector& vec1, const Vector& vec2);
Vector projectedPerpendicularVector(const Vector& vec1, const Vector& vec2);
Vector surfaceNormal(const Vector& vec1, const Vector& vec2);
double distance(const Vector& vec1, const Vector& vec2);
double angle(const Vector& vec1, const Vector& vec2);

}  // namespace CapEngine
#endif

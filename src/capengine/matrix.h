// \brief Class that defined matrix type and functions
// \author Caleb Palmer
#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"
#include <vector>

typedef std::vector<CapEngine::Vector> MatrixContainer;

namespace CapEngine
{

// should the vectors be stored as column vectors or row vectors?
// assume row vectors
class Matrix
{

  public:
	Matrix(CapEngine::Vector vec1, CapEngine::Vector vec2,
		   CapEngine::Vector vec3, CapEngine::Vector vec4);
	~Matrix() = default;

	Vector getColumnVector(int index) const;
	Vector getRowVector(int index) const;
	Vector &getColumnVectorRef(int index);

	void setRowVector(int index, Vector vector);

	static Matrix createZeroMatrix();
	static Matrix createIdentityMatrix();
	static Matrix createTranslationMatrix(double x, double y, double z);
	static Matrix createScaleMatrix(double x, double y, double z);
	static Matrix createXRotationMatrix(double degrees);
	static Matrix createYRotationMatrix(double degrees);
	static Matrix createZRotationMatrix(double degrees);

	std::string toString(bool pretty = false) const;

	// overloaded operators
	Matrix operator*(const Matrix &right) const;
	Matrix operator+(const Matrix &right) const;
	Matrix operator-(const Matrix &right) const;
	Vector operator*(const Vector &right) const;
	bool operator==(const Matrix &other) const;
	std::string pretty() const;
	friend std::ostream &operator<<(std::ostream &stream, const Matrix &matix);

	std::unique_ptr<float> getGLMatrix() const;
	const MatrixContainer &getVectors() const;

  protected:
	MatrixContainer vectors;
};

} // namespace CapEngine
#endif // MATRIX_H

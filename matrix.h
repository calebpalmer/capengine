// \brief Class that defined matrix type and functions
// \author Caleb Palmer
#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include "vector.h"

typedef std::vector<CapEngine::Vector> MatrixContainer;

namespace CapEngine {
  
  // should the vectors be stored as column vectors or row vectors?
  // assume row vectors
  class Matrix {

  public:
    Matrix(CapEngine::Vector vec1, CapEngine::Vector vec2, CapEngine::Vector vec3, CapEngine::Vector vec4);
    ~Matrix() = default;
    
    Vector getColumnVector(int index) const;
    Vector getRowVector(int index) const;
		Vector& getColumnVectorRef(int index);

    void setRowVector(int index, Vector vector);

    static Matrix createZeroMatrix();
    static Matrix createIdentityMatrix();
    static Matrix createTranslationMatrix(real x, real y, real z);
    static Matrix createScaleMatrix(real x, real y, real z);
    static Matrix createXRotationMatrix(real degrees);
    static Matrix createYRotationMatrix(real degrees);
    static Matrix createZRotationMatrix(real degrees);

		std::string toString(bool pretty = false) const;

    // overloaded operators
    Matrix operator*(const Matrix& right) const;
    Matrix operator+(const Matrix& right) const;
    Matrix operator-(const Matrix& right) const;    
    Vector operator*(const Vector& right) const;
		bool operator==(const Matrix& other) const;
    friend std::ostream& operator<<(std::ostream& stream, const Matrix& matix);
		
    std::unique_ptr<float> getGLMatrix() const;
    const MatrixContainer& getVectors() const;
  protected:
    MatrixContainer vectors;
  };
 
}
#endif //MATRIX_H

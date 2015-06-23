// \brief Class that defined matrix type and functions
// \author Caleb Palmer
#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include "vector.h"

typedef std::vector<CapEngine::Vector*> MatrixContainer;

namespace CapEngine {
  
  // should the vectors be stored as column vectors or row vectors?
  // assume row vectors
  class Matrix {

  public:
    Matrix(CapEngine::Vector& vec1, CapEngine::Vector& vec2, CapEngine::Vector& vec3, CapEngine::Vector& vec4);
    ~Matrix();
    
    Vector& getColumnVector(int index) const;
    Vector& getRowVector(int index) const ;

    static Matrix* createIdentityMatrix();
    static Matrix* createTranslationMatrix(real x, real y, real z);
    static Matrix* createScaleMatrix(real x, real y, real z);
    static Matrix* createXRotationMatrix(real degrees);
    static Matrix* createYRotationMatrix(real degrees);
    static Matrix* createZRotationMatrix(real degrees);


    // overloaded operators
    Matrix& operator*(const Matrix& left);
    
    float* getGLMatrix() const;
    const MatrixContainer& getVectors() const;
  protected:
    MatrixContainer vectors;
  };
 
}
#endif //MATRIX_H

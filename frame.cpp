#include "frame.h"

using namespace std;
using namespace CapEngine;

Frame::Frame(CapEngine::Vector &locationIn, CapEngine::Vector &upIn,
             CapEngine::Vector &forwardIn)
{
  location.reset(&locationIn);
  up.reset(&upIn);
  forward.reset(&forwardIn);
}

// WRONG! Need to remplement
// https://webcache.googleusercontent.com/search?q=cache:XqVxBFNwoDEJ:db-in.com/blog/2011/04/cameras-on-opengl-es-2-x/+&cd=1
Matrix *Frame::getMatrix()
{

  unique_ptr<Vector> xVector(up->crossProduct(*forward.get()));

  unique_ptr<Matrix> translationMatrix(Matrix::createTranslationMatrix(
      location->getX(), location->getY(), location->getZ()));
  unique_ptr<Matrix> xRotationMatrix(Matrix::createXRotationMatrix());
  unique_ptr<Matrix> yRotationMatrix(Matrix::createYRotationMatrix());
  unique_ptr<Matrix> zRotationMatrix(Matrix::createZRotationMatrix());
}

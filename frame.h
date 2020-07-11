#ifndef FRAME_H
#define FRAME_H

#include <memory>

#include "matrix.h"
#include "vector.h"

namespace CapEngine
{

class Frame
{

public:
  Frame(CapEngine::Vector &locationIn, CapEngine::Vector &upIn,
        CapEngine::Vector &forwardIn);
  CapEngine::Matrix *getMatrix();

private:
  std::unique_ptr<Vector> location;
  std::unique_ptr<Vector> up;
  std::unique_ptr<Vector> forward;
};

} // namespace CapEngine

#endif

#ifndef FRAME_H
#define FRAME_H

#include <memory>

#include "vector.h"
#include "matrix.h"

namespace CapEngine {
  
  class Frame{

  public:
    Frame(CapEngine::Vector& locationIn, CapEngine::Vector& upIn, CapEngine::Vector& forwardIn);
    CapEngine::Matrix* getMatrix();

  private:
    std::unique_ptr<Vector> location;
    std::unique_ptr<Vector> up;
    std::unique_ptr<Vector> forward;
    
  };
  

}


#endif

#ifndef TIMESTEP_H
#define TIMESTEP

#include "Time.h"
#include "captypes.h"

// timestep ms = current time - lastupdatetime - lastrendertime

namespace CapEngine{
  class TimeStep{
    Time lastUpdateTime;
    Time lastRenderTime;
    
    TimeStep(const TimeStep&);
    TimeStep& operator=(const TimeStep&);

  public:
    real lastTimeStep;

    TimeStep();
    real updateStep();
    void renderStep();
  };

}

#endif

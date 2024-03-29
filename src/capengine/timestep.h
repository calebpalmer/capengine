#ifndef TIMESTEP_H
#define TIMESTEP_H

#include "Time.h"
#include "captypes.h"

// timestep ms = current time - lastupdatetime - lastrendertime

namespace CapEngine
{
class TimeStep
{
  Time lastUpdateTime;
  Time lastRenderTime;

  TimeStep(const TimeStep &);
  TimeStep &operator=(const TimeStep &);

public:
  double lastTimeStep;

  TimeStep();
  double updateStep();
  void renderStep();
};

} // namespace CapEngine

#endif

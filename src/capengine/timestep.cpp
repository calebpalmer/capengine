#include "timestep.h"

using namespace CapEngine;

TimeStep::TimeStep() { lastTimeStep = 0.0; }

real TimeStep::updateStep()
{

  Time oldUpdateTime(lastUpdateTime);
  lastUpdateTime.reset();

  if (lastTimeStep == 0.0) {
    lastTimeStep = -1.0;
    return lastTimeStep;
  } else {
    // new update time - last update time - (last render time - last update
    // time)
    // double renderDiff = lastRenderTime.subtractTime(&oldUpdateTime);
    double renderDiff = 0.0; // TODO this needs to be fixed.  Above commented
                             // line is not working for some reason.
    lastTimeStep = lastUpdateTime.subtractTime(&oldUpdateTime) - renderDiff;
    return lastTimeStep;
  }
}

void TimeStep::renderStep() { lastRenderTime.reset(); }

//@brief - implementation for Time class
//@author - Caleb Palmer

#include "Time.h"
#include "CapEngineException.h"

using namespace CapEngine;

Time::Time()
{
  if (gettimeofday(&time, NULL) == -1) {
    throw CapEngineException("Unable to get current time");
  }
}

Time::Time(const Time &copiedTime) { time = copiedTime.time; }

Time::~Time() {}

double Time::subtractTime(const Time *timeToSubtract)
{
  if (timeToSubtract == NULL) {
    return 0;
  }

  return ((time.tv_sec * 1000) + (time.tv_usec / 1000)) -
         ((timeToSubtract->time.tv_sec * 1000) +
          (timeToSubtract->time.tv_usec / 1000));
}

double Time::elapsedTime()
{
  timeval curTime;
  if (gettimeofday(&curTime, NULL) == -1) {
    throw CapEngineException("Unable to get current time");
  }

  return ((curTime.tv_sec * 1000) + (curTime.tv_usec / 1000)) -
         ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void Time::reset()
{
  if (gettimeofday(&time, NULL) == -1) {
    throw CapEngineException("Unable to get current time");
  }
}

Time &Time::operator=(const Time &timeToCopy)
{
  this->time = timeToCopy.time;
  return *this;
}

double CapEngine::currentTime()
{
  struct timeval time;

  if (gettimeofday(&time, NULL) == -1) {
    throw CapEngineException("Unable to get current time");
  }

  return (time.tv_sec * 1000) + (time.tv_usec / 1000);
}

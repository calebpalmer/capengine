//@brief - header file for TimeManager class
//@author - Caleb Palmer

#ifndef TIMECLASS_H
#define TIMECLASS_H

#include <sys/time.h>
#include <stdexcept>
#include <string>

namespace CapEngine {

  class Time {
  private:
    Time& operator=(const Time&){ return *this; }

  protected:
    struct timeval time;
  
  public:
    Time();
    ~Time();
    Time(const Time& updateTime);
	
    //Subtract time from this and return microseconds
    //@param timeToSubtract - subtract this from current time object
    //@return elapsed time in microseconds
    double subtractTime(const Time* timeToSubtract);

    // returns elapsed time of current time - stored time
    // @return time elapsed in milliseconds
    double elapsedTime();

    // set Time to currentTime
    void reset();

  };

}
#endif

#ifndef ANIMATION_H
#define ANIMATION_H

namespace CapEngine {

  class Animation{
  public:
    virtual ~Animation(){}
    virtual void update(double deltaTimeMS) = 0;
    virtual void render() = 0;
    virtual bool isDone() = 0;
  };
}


#endif //ANIMATION_H

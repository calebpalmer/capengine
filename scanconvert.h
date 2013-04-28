#ifndef SCANCONVERT_H
#define SCANCONVERT_H

#include "captypes.h"

namespace CapEngine {

  static Uint8 scanR = 0xff;
  static Uint8 scanG = 0xff;
  static Uint8 scanB = 0xff;

  void drawLine(int x0, int y0, int x1, int y1, Surface* surface);
  void writePixel(CapEngine::Surface* surface, int x, int y);

}
#endif // SCANCONVERT_H

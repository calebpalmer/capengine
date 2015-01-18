#ifndef SCANCONVERT_H
#define SCANCONVERT_H

#include "captypes.h"

namespace CapEngine {

  const Uint8 scanR = 0xff;
  const Uint8 scanG = 0xff;
  const Uint8 scanB = 0xff;

  enum EdgePattern {
    SolidEdge,
    StripedEdge
  };

  void drawLine(int x0, int y0, int x1, int y1, Surface* surface, EdgePattern pattern=SolidEdge);
  void writePixel(CapEngine::Surface* surface, int x, int y);

  static int basicStippleLength = 10;

}
#endif // SCANCONVERT_H

#ifndef SCANCONVERT_H
#define SCANCONVERT_H

#include "captypes.h"
#include "colour.h"
#include "vector.h"

#include <boost/optional.hpp>

namespace CapEngine {

  const Uint8 scanR = 0x00;
  const Uint8 scanG = 0x00;
  const Uint8 scanB = 0x00;

  enum class EdgePattern {
    SolidEdge,
    StripedEdge
  };

  struct Pixel{
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;
  };

  void drawLine(int x0, int y0, int x1, int y1, Surface* surface, EdgePattern pattern=EdgePattern::SolidEdge);
  void drawRectangle(Surface* surface, Rect rectangle);
  Surface* createRectangle(int width, int height, Colour colour);
  Surface* createRectangle2(int width, int height, Colour colour);
  void writePixel(CapEngine::Surface* surface, int x, int y);
  void writePixel(CapEngine::Surface*, int x, int y, CapEngine::Colour colour);
  Uint32 getPixel(const CapEngine::Surface* surface, int x, int y);
  void getPixelComponents(const CapEngine::Surface* surface, int x, int y, Uint8* r, Uint8* g, Uint8* b, Uint8* a);
  Pixel getPixelComponents(const CapEngine::Surface* surface, int x, int y);
  boost::optional<Vector> getTangent(const CapEngine::Surface* surface, int x, int y, Pixel solidPixel, int numNeighbours, bool above=true);
  
  void writePixel(Uint32* buffer, int x, int y, CapEngine::Colour colour, int bufWidth, int bufHeight);

  static int basicStippleLength = 10;

}
#endif // SCANCONVERT_H

#include "tools.h"

#include "CapEngineException.h"
#include "capcommon.h"
#include "locator.h"
#include "scanconvert.h"

namespace CapEngine
{
/*
  Creates an ASCII representation of a surface
 */
std::string surfaceToASCII(Surface *surface)
{
  CAP_THROW_NULL(surface, "Provided surface is null");
  CAP_THROW_NULL(Locator::videoManager,
				 "Can't locate VideoManager.  Has engine been initialized?");
  VideoManager &vMan = *Locator::videoManager;

  double width = 0;
  double height = 0;
  width = vMan.getSurfaceWidth(surface);
  height = vMan.getSurfaceHeight(surface);

  std::ostringstream repr;
  for (int j = 0; j < height; j++) {
	for (int i = 0; i < width; i++) {
	  Pixel pixel =
		  getPixelComponents(surface, i, vMan.toScreenCoord(surface, j));
	  repr << std::dec << i << ", " << j << ": 0x" << std::hex
		   << static_cast<int>(pixel.r) << " 0x" << static_cast<int>(pixel.g)
		   << " 0x" << static_cast<int>(pixel.b) << " 0x"
		   << static_cast<int>(pixel.a) << std::endl;
	}
  }

  return repr.str();
}
} // namespace CapEngine

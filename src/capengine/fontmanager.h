#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include "VideoManager.h"
#include "captypes.h"
#include "colour.h"
#include <SDL2/SDL_ttf.h>
#include <map>
#include <string>

namespace CapEngine
{

// TODO: this doesn't need to be a class
//! Class for creating text surfaces
class FontManager
{
public:
  FontManager();
  ~FontManager();


  SurfacePtr getTextSurface(const std::string &font, const std::string &text,
                          int fontSize, Uint8 r, Uint8 g, Uint8 b) const;
  SurfacePtr getTextSurface(const std::string &font, const std::string &text,
                          int fontSize, Colour colour) const;
  SurfacePtr getTextSurface(const std::string &font, const std::string &text,
                            int fontSize, Colour colour);

}; // FontManager

int getFontHeight(const std::string &font, int fontSize);

} // namespace CapEngine

#endif

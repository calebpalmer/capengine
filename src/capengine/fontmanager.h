#ifndef CAPENGINE_FONTMANAGER_H
#define CAPENGINE_FONTMANAGER_H

#include <SDL2/SDL_ttf.h>

#include <map>
#include <string>

#include "VideoManager.h"
#include "captypes.h"
#include "colour.h"

namespace CapEngine
{

//! Class for creating text surfaces
class FontManager
{
   public:
    FontManager();
    ~FontManager();

    [[nodiscard]] SurfacePtr getTextSurface(const std::string &font,
                                            const std::string &text,
                                            int fontSize, Uint8 r, Uint8 g,
                                            Uint8 b) const;
    [[nodiscard]] SurfacePtr getTextSurface(const std::string &font,
                                            const std::string &text,
                                            int fontSize, Colour colour) const;
    [[nodiscard]] SurfacePtr getTextSurface(const std::string &font,
                                     const std::string &text, int fontSize,
                                     int surfaceWidth, int surfaceHeight) const;
};  // FontManager

int getFontHeight(const std::string &font, int fontSize);

}  // namespace CapEngine

#endif // CAPENGINE_FONTMANAGER_H

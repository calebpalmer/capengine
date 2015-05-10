#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include <string>
#include <map>
#include <SDL2/SDL_ttf.h>
#include "captypes.h"
#include "VideoManager.h"

namespace CapEngine {

  class FontManager{
  public:
    FontManager();
    ~FontManager();
    Surface* getTextSurface(const std::string& font, const std::string& text, int fontSize
			    ,int surfaceWidth=0, int surfaceHeight=0) const; 
    Surface* getTextSurface(const std::string& font, const std::string& text, int fontSize
			    , Uint8 r, Uint8 g, Uint8 b) const; 

  }; //FontManager

} //CapEngine

#endif

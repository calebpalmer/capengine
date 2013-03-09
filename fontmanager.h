#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include <string>
#include <map>
#include <SDL/SDL_ttf.h>
#include "captypes.h"
#include "VideoManager.h"

namespace CapEngine {

  class FontManager{
  public:
    FontManager();
    ~FontManager();
    Surface* getTextSurface(const std::string& font, const std::string& text, int fontSize
			    ,int surfaceWidth=0, int surfaceHeight=0) const; 

  }; //FontManager

} //CapEngine

#endif

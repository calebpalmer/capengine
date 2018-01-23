#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include <string>
#include <map>
#include <SDL2/SDL_ttf.h>
#include "captypes.h"
#include "VideoManager.h"
#include "colour.h"

namespace CapEngine {

	// TODO: this doesn't need to be a class
	//! Class for creating text surfaces
	class FontManager{
  public:
    FontManager();
    ~FontManager();
    Surface* getTextSurface(const std::string& font, const std::string& text, int fontSize
			    ,int surfaceWidth=0, int surfaceHeight=0) const; 
    Surface* getTextSurface(const std::string& font, const std::string& text, int fontSize
			    , Uint8 r, Uint8 g, Uint8 b) const;
    Surface* getTextSurface(const std::string& font, const std::string& text, int fontSize
			    , Colour colour) const; 

  }; //FontManager

} //CapEngine

#endif

#include "fontmanager.h"
#include "CapEngineException.h"

#include <SDL/SDL_ttf.h>

using namespace std;

namespace CapEngine {

  //! Font Manager constructor
  /*! initialise SDL_ttf
   */
  FontManager::FontManager(const VideoManager& vManagerIn) : vManager(&vManagerIn){
    if (TTF_Init() != 0){
      throw new CapEngineException("Unable to initialise SDL_ttf");
    }
  }
  
  FontManager::~FontManager(){
    TTF_Quit();
  }

  Surface* FontManager::getTextSurface(const std::string& font, const std::string& text
				       ,int fontSize, int surfaceWidth, int surfaceHeight) const{
    
    TTF_Font* fontFace;
    string fontPath(font);
    fontFace = TTF_OpenFont(font.c_str(), fontSize);
    if(fontFace == 0){
      string ttf_error = TTF_GetError();
      TTF_CloseFont(fontFace);
      throw new CapEngineException("Unable to open font: " + ttf_error);
      return nullptr;
    }

    SDL_Surface* fontSurface;
    SDL_Color fontColor;
    fontColor.r = 0;
    fontColor.g = 0;
    fontColor.b = 0;
  
    fontSurface = TTF_RenderText_Solid(fontFace, text.c_str(), fontColor);
    if(fontSurface == 0){
      string ttf_error = TTF_GetError();
      TTF_CloseFont(fontFace);
      throw new CapEngineException("Unable to get text surface: " + ttf_error);
    }

    TTF_CloseFont(fontFace);
    return fontSurface;

  }
}

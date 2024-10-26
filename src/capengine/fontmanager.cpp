#include "fontmanager.h"
#include "CapEngineException.h"
#include "captypes.h"
#include "defer.h"

#include <sstream>

#include <SDL2/SDL_ttf.h>

using namespace std;

namespace CapEngine
{

namespace
{

//! Opens a font
/**
 \param font
   \li The path to the ttf font
 \param fontSize
   \li The size of the font
 \return
   \li The font
*/
std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)>
    openFont(const std::string &font, int fontSize)
{
  TTF_Font *fontFace;
  string fontPath(font);
  fontFace = TTF_OpenFont(font.c_str(), fontSize);

  if (fontFace == nullptr) {
    string ttf_error = TTF_GetError();
    TTF_CloseFont(fontFace);
    stringstream errorMsg;
    errorMsg << "Unable to open font: " << font << endl
             << "\tDetails: " << ttf_error;
    BOOST_THROW_EXCEPTION(CapEngineException(errorMsg.str()));
  }

  return unique_ptr<TTF_Font, decltype(&TTF_CloseFont)>(fontFace,
                                                        TTF_CloseFont);
}

} // namespace

//! Font Manager constructor
/*! initialise SDL_ttf
 */
FontManager::FontManager()
{
  if (TTF_Init() != 0) {
    throw CapEngineException("Unable to initialise SDL_ttf");
  }
}

SurfacePtr getTextSurface(const std::string &font, const std::string &text,
                          int fontSize, Colour colour){
    TTF_Font *fontFace = nullptr;
    fontFace = TTF_OpenFont(font.c_str(), fontSize);
    if (fontFace == 0) {
        string ttf_error = TTF_GetError();
        TTF_CloseFont(fontFace);
        stringstream errorMsg;
        errorMsg << "Unable to open font: " << font << endl
                 << "\tDetails: " << ttf_error;
        throw CapEngineException(errorMsg.str());
        return getNullSurfacePtr();;
    }
    Defer defer([&fontFace](){TTF_CloseFont(fontFace);});

    SDL_Surface *fontSurface = nullptr;
    SDL_Color fontColor;
    fontColor.r = colour.m_r;
    fontColor.g = colour.m_g;
    fontColor.b = colour.m_b;

    fontSurface = TTF_RenderText_Solid(fontFace, text.c_str(), fontColor);
    if (fontSurface == 0) {
        string ttf_error = TTF_GetError();
        TTF_CloseFont(fontFace);
        throw CapEngineException("Unable to get text surface: " + ttf_error);
    }

    return SurfacePtr{fontSurface, SDL_FreeSurface};
}

FontManager::~FontManager() { TTF_Quit(); }

// SurfacePtr FontManager::getTextSurface(const std::string &font,
//                                      const std::string &text, int fontSize,
//                                      int surfaceWidth, int surfaceHeight) const
// {
//   return getTextSurface(font, text, fontSize, 0, 0, 0);
// }

SurfacePtr FontManager::getTextSurface(const std::string &font,
                                     const std::string &text, int fontSize,
                                     Colour colour) const
{
  return getTextSurface(font, text, fontSize, colour.m_r, colour.m_g,
                        colour.m_b);
}



//! Get the height in pixels of the given font.
/**
 \param font
   \li The path to the ttf font.
 \param fontSize
   \li the font size
 \return
   \li The height of the font in pixels.
*/
int getFontHeight(const std::string &font, int fontSize)
{
  // TODO finish implementing this.
  return 0;
}

} // namespace CapEngine

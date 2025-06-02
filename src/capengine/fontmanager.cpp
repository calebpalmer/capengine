#include "fontmanager.h"

#include <SDL2/SDL_ttf.h>

#include <memory>
#include <sstream>

#include "CapEngineException.h"
#include "captypes.h"
#include "defer.h"
#include "locator.h"

using namespace std;

namespace CapEngine {

namespace {

using FontPtr = std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)>;

//! Opens a font
/**
 \param font
   \li The path to the ttf font
 \param fontSize
   \li The size of the font
 \return
   \li The font
*/
FontPtr openFont(const std::string& font, int fontSize)
{
    TTF_Font* fontFace;
    fontFace = TTF_OpenFont(font.c_str(), fontSize);

    if (fontFace == nullptr) {
        // next try with asset manager base location if it has one
        std::optional<std::filesystem::path> basePath = Locator::getAssetManager().getBasePath();
        if (basePath.has_value()) {
            const std::string fontPath = *basePath / "fonts" / font;
            fontFace = TTF_OpenFont(fontPath.c_str(), fontSize);

            // if it's still null after checking the asset manager basepath then throw an exception
            if (fontFace == nullptr) {
                string ttf_error = TTF_GetError();
                TTF_CloseFont(fontFace);
                stringstream errorMsg;
                errorMsg << "Unable to open font: " << font << endl << "\tDetails: " << ttf_error;
                BOOST_THROW_EXCEPTION(CapEngineException(errorMsg.str()));
            }
        }
    }

    return FontPtr(fontFace, TTF_CloseFont);
}

}  // namespace

//! Font Manager constructor
/*! initialise SDL_ttf
 */
FontManager::FontManager()
{
    if (TTF_Init() != 0) {
        throw CapEngineException("Unable to initialise SDL_ttf");
    }
}

FontManager::~FontManager() { TTF_Quit(); }

//! Get the height in pixels of the given font.
/**
 \param font
   \li The path to the ttf font.
 \param fontSize
   \li the font size
 \param text
   \li The text to render
 \param colour
   \li The colour of the text
 \return
   \li The surface containing the text.
*/
SurfacePtr FontManager::getTextSurface(const std::string& font, const std::string& text, int fontSize,
                                       Colour colour) const
{
    FontPtr fontFace = openFont(font, fontSize);

    SDL_Surface* fontSurface = nullptr;
    SDL_Color fontColor;
    fontColor.r = colour.m_r;
    fontColor.g = colour.m_g;
    fontColor.b = colour.m_b;

    fontSurface = TTF_RenderText_Solid(fontFace.get(), text.c_str(), fontColor);
    if (fontSurface == 0) {
        string ttf_error = TTF_GetError();
        TTF_CloseFont(fontFace.get());
        throw CapEngineException("Unable to get text surface: " + ttf_error);
    }

    return SurfacePtr{fontSurface, SDL_FreeSurface};
}

//! Get the height in pixels of the given font.
/**
 \param font
   \li The path to the ttf font.
 \param fontSize
   \li the font size
 \param text
   \li The text to render
 \param r
   \li The red component of the colour
 \param g
   \li The green component of the colour
 \param b
   \li The blue component of the colour
 \return
   \li The surface containing the text.
*/
SurfacePtr FontManager::getTextSurface(const std::string& font, const std::string& text, int fontSize, Uint8 r, Uint8 g,
                                       Uint8 b) const
{
    return getTextSurface(font, text, fontSize, Colour{r, g, b});
}

//! Get the height in pixels of the given font.
/**
 \param font
   \li The path to the ttf font.
 \param fontSize
   \li the font size
 \param text
   \li The text to render
 \return
   \li The surface containing the text.
*/
SurfacePtr FontManager::getTextSurface(const std::string& font, const std::string& text, int fontSize, int surfaceWidth,
                                       int surfaceHeight) const
{
    // TODO this ain't right
    return getTextSurface(font, text, fontSize, 0, 0, 0);
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
int getFontHeight(const std::string& font, int fontSize)
{
    // TODO finish implementing this.
    return 0;
}

}  // namespace CapEngine

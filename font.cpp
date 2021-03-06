#include "font.h"

#include "CapEngineException.h"

#include <sstream>

namespace CapEngine
{

//! Constructor
/**
 \param fontPath
   \li The path to the TTF font to use.
 \param
   \li The size of the font.
*/
Font::Font(const std::string &fontPath, int size)
    : m_pTTFFont(nullptr, TTF_CloseFont)
{
  TTF_Font *pFont;
  pFont = TTF_OpenFont(fontPath.c_str(), size);

  if (pFont == nullptr) {
    std::string ttf_error = TTF_GetError();
    std::stringstream errorMsg;
    errorMsg << "Unable to open font: " << fontPath << std::endl
             << "\tDetails: " << ttf_error;
    BOOST_THROW_EXCEPTION(CapEngineException(errorMsg.str()));
  }

  m_pTTFFont =
      std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)>(pFont, TTF_CloseFont);
}

//! Get the height of the font.
/**
 \return
   \li The height of the font.
*/
int Font::getFontHeight() const
{
  assert(m_pTTFFont != nullptr);
  return TTF_FontHeight(m_pTTFFont.get());
}

//! Render some text.
/**
 \param text
   \li The text to render.
 \param colour
   \li The foreground colour.
 \param bgColour
   \li Optional background colour.
 \return
*/
SurfacePtr Font::getTextSurface(const std::string &text, const Colour &colour,
                                std::optional<Colour> bgColour)
{
  SDL_Colour sdlColour{colour.m_r, colour.m_g, colour.m_b, colour.m_a};
  SurfacePtr surface = getNullSurfacePtr();

  if (bgColour == std::nullopt) {
    surface.reset(
        TTF_RenderText_Solid(m_pTTFFont.get(), text.c_str(), sdlColour));
  }

  else {
    SDL_Colour sdlBgColour{bgColour->m_r, bgColour->m_g, bgColour->m_b,
                           bgColour->m_a};
    surface.reset(TTF_RenderText_Shaded(m_pTTFFont.get(), text.c_str(),
                                        sdlColour, sdlBgColour));
  }

  return surface;
}

//! Get the width of the text if rendered with this font.
/**
 \return
   \li The width of the rendered text.
*/
int Font::getTextSize(const std::string &text) const
{
  assert(m_pTTFFont != nullptr);

  int width = 0;
  int height = 0;

  if (TTF_SizeText(m_pTTFFont.get(), text.c_str(), &width, &height)) {
    BOOST_THROW_EXCEPTION(
        CapEngineException(std::string("Font could not be rendered: ") +
                           std::string(TTF_GetError())));
  }

  return width;
}

} // namespace CapEngine

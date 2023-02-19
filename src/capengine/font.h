#ifndef CAPENGINE_FONT_H
#define CAPENGINE_FONT_H

#include "captypes.h"
#include "colour.h"

#include <SDL2/SDL_ttf.h>
#include <memory>
#include <optional>
#include <string>

namespace CapEngine
{

class Font
{
public:
  Font(const std::string &fontPath, int size);

  Font(const Font &) = delete;
  Font &operator=(const Font &) = delete;

  Font(Font &&other) = default;
  Font &operator=(Font &&other) = default;

  int getFontHeight() const;
  SurfacePtr getTextSurface(const std::string &text, const Colour &colour,
                            std::optional<Colour> bgColour = std::nullopt);
  int getTextSize(const std::string &text) const;

private:
  std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)> m_pTTFFont;
};

} // namespace CapEngine

#endif // CAPENGINE_FONT_H

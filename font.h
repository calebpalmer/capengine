#ifndef CAPENGINE_FONT_H
#define CAPENGINE_FONT_H

#include <string>
#include <memory>
#include <SDL2/SDL_ttf.h>

namespace CapEngine {

	class Font {
	public:
		Font(const std::string &fontPath, int size);

		Font(const Font&) = delete;
		Font& operator=(const Font&) = delete;
		
		Font(Font &&other) = default;
		Font& operator=(Font &&other) = default;

		int getFontHeight() const;

	private:
		std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)> m_pTTFFont;
	};

} // namespace CapEngine

#endif // CAPENGINE_FONT_H

#include "font.h"

#include "CapEngineException.h"

#include <sstream>

namespace CapEngine {

//! Constructor
/** 
 \param fontPath
   \li The path to the TTF font to use.
 \param 
   \li The size of the font.
*/
Font::Font(const std::string &fontPath, int size) :
	m_pTTFFont(nullptr, TTF_CloseFont)
{
	TTF_Font* pFont;
	pFont = TTF_OpenFont(fontPath.c_str(), size);

	if(pFont == nullptr){
		std::string ttf_error = TTF_GetError();
		TTF_CloseFont(pFont);
		std::stringstream errorMsg;
		errorMsg << "Unable to open font: " << fontPath << std::endl
						 << "\tDetails: " << ttf_error;
		BOOST_THROW_EXCEPTION(CapEngineException(errorMsg.str()));
	}

	m_pTTFFont = std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)>(pFont, TTF_CloseFont);
}


//! Get the height of the font.
/** 
 \return 
   \li The height of the font.
*/
int Font::getFontHeight() const {
	assert(m_pTTFFont != nullptr);
	return TTF_FontHeight(m_pTTFFont.get());
}

}

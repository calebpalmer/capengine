#include "label.h"

#include "VideoManager.h"
#include "locator.h"
#include "CapEngineException.h"
#include "utils.h"
#include "uiconfigmanager.h"

namespace CapEngine { namespace UI {


//! Constructor
/**
 \param pParent - The parent widget
 \param text - The text to display
 \param font
   \li The ttf font to render in (full path to TTF file)
	 \li If empty, the default will be retrieved.
 */
Label::Label(const std::string &text, const std::string &font)
	: m_text(text), m_font(font), m_texture(getNullTexturePtr())
{
	if(m_font.empty()){
		boost::optional<std::string> maybeFont = UIConfigManager::getInstance().getSetting(kDefaultFontSettingsPath);
		if(!maybeFont){
			CAP_THROW(CapEngineException("Default font setting not found"));
		}

		m_font = *maybeFont;
	}
}


//! creates a label
/** 
 \param text - The text of the label
 \param font - The path to the ttf font file
 \return - The label
*/
std::shared_ptr<Label> Label::create(const std::string &text, const std::string &font){
	std::shared_ptr<Label> pLabel(new Label(text, font));
	return pLabel;
}

//! @copydoc Widget::setPosition()
void Label::setPosition(int x, int y){
	m_x = x;
	m_y = y;
}

//! @copydoc Widget::setSize()
void Label::setSize(int width, int height){
	if(width != m_width || height != m_height){
		m_width = width;
		m_height = height;
	}

}

//! @copydoc Widget::render()
void Label::render() {
	CAP_THROW_ASSERT(Locator::videoManager != nullptr, "VideoManager is null");
	CAP_THROW_ASSERT(m_windowId != VideoManager::kInvalidWindowId, "Invalid window id");

	if(!m_texture){
		const int fontSize = 128;

		CAP_THROW_ASSERT(Locator::videoManager != nullptr, "VideoManager is null");
		CAP_THROW_ASSERT(m_windowId != VideoManager::kInvalidWindowId, "Invalid window id");

		SDL_Surface *surface =
			m_fontManager.getTextSurface(m_font, m_text, fontSize);
		
		m_texture = Locator::videoManager->createTextureFromSurfacePtr(m_windowId, surface, true);
		CAP_THROW_ASSERT(m_texture != nullptr, "Unable to create texture");
	}

	int textureWidth = 0;
	int textureHeight = 0;
	Locator::videoManager->getTextureDims(m_texture.get(), &textureWidth, &textureHeight);

	SDL_Rect srcRect = {0, 0, textureWidth, textureHeight};
	//	SDL_Rect dstRect = { m_x, m_y, m_width, m_height };
	SDL_Rect dstRect = expandRectToFit(srcRect, {m_x, m_y, m_width, m_height});
	Locator::videoManager->drawTexture(m_windowId, m_texture.get(), &srcRect, &dstRect);
}

}} // namespace CapEngine::UI

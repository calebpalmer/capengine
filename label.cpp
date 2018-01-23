#include "label.h"

#include "VideoManager.h"
#include "locator.h"
#include "CapEngineException.h"

namespace CapEngine { namespace UI {


//! Constructor
/**
 * \param pParent - The parent widget
 * \param text - The text to display
 * \param font - The ttf font to render in (full path to TTF file)
 */
Label::Label(Widget* pParent, const std::string &text, const std::string &font)
	: Widget(pParent), m_text(text), m_font(font), m_texture(getNullTexturePtr()) {

		int fontSize = 20;
		SDL_Surface *surface =
			m_fontManager.getTextSurface(m_font, m_text, fontSize);
		
		CAP_THROW_ASSERT(Locator::videoManager != nullptr, "VideoManager is null");
		m_texture = Locator::videoManager->createTextureFromSurfacePtr(surface, true);
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
	CAP_THROW_ASSERT(m_texture != nullptr, "Label texture is null");
	CAP_THROW_ASSERT(m_windowId != VideoManager::kInvalidWindowId, "Invalid window id");

	int textureWidth = 0;
	int textureHeight = 0;
	Locator::videoManager->getTextureDims(m_texture.get(), &textureWidth, &textureHeight);
	
	SDL_Rect srcRect = { 0, 0, m_width, m_height };
	SDL_Rect dstRect = { m_x, m_y, m_width, m_height };
	Locator::videoManager->drawTexture(m_windowId, m_texture.get(), &srcRect, &dstRect);
}



}} // namespace CapEngine::UI

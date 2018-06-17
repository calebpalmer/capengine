#include "tilecopycontrol.h"

#include "locator.h"
#include "CapEngineException.h"
#include "scanconvert.h"
#include "widgetstate.h"
#include "uiutils.h"

#include <boost/numeric/conversion/cast.hpp>
#include <boost/lexical_cast.hpp>

namespace CapEngine { namespace UI {


//!  Getter for Tileset
/** 
 \return - The Tileset
*/
std::shared_ptr<const TileSet> TileCopyControl::getTileSet() const {
	return m_pTileSet;
}



//! Getter for the tile index
/** 
 \return - The index of the tile.
*/
int TileCopyControl::getIndex() const{
	return m_index;
}


//! \copydoc Widget::render
void TileCopyControl::render(){
	if(!m_pTexture)
		loadTexture();

	int x = 0;
	int y = 0;
	SDL_GetMouseState(&x, &y);

	int width = m_pTileSet->getTileWidth();
	int height = m_pTileSet->getTileHeight();

	SDL_Rect srcRect = {0, 0, width, height};
	
	SDL_Rect dstRect = {
		x - static_cast<int>((static_cast<float>(width) / 2.0)),
		y - static_cast<int>((static_cast<float>(height) / 2.0)),
		width,
		height
	};

	assert(Locator::videoManager != nullptr);
	Locator::videoManager->drawTexture(m_windowId, m_pTexture.get(), &srcRect, &dstRect);
}


//! load the texture that gets drawn with the cursor
void TileCopyControl::loadTexture(){
	assert(Locator::videoManager != nullptr);
	assert(m_pTileSet != nullptr);

	auto tileWidth = m_pTileSet->getTileWidth();
	auto tileHeight = m_pTileSet->getTileHeight();
	
	SurfacePtr pSurface = Locator::videoManager->createSurfacePtr(tileWidth, tileHeight);
	assert(pSurface != nullptr);

	std::shared_ptr<SDL_Surface> pTileSetSurface = m_pTileSet->getSurface();
	assert(pTileSetSurface != nullptr);

	const std::vector<Tile> &tiles = m_pTileSet->getTiles();
	if(boost::numeric_cast<unsigned int>(m_index) >= tiles.size()){
		BOOST_THROW_EXCEPTION(CapEngineException("Tile index out of bounds:" + boost::lexical_cast<std::string>(m_index)));
	}

	const Tile& tile = tiles.at(m_index);
	Locator::videoManager->blitSurface(pTileSetSurface.get(), tile.xpos, tile.ypos, tileWidth, tileHeight, pSurface.get(), 0, 0);

	// add alpha for some transparency
	uint8_t alpha = 128;
	for(size_t y = 0; y < tileWidth; y++){
		for(size_t x = 0; x < tileHeight; x++){
			Pixel pixel = getPixelComponents(pSurface.get(), x, y);
			pixel.a = alpha;
			writePixel(pSurface.get(), x, y, pixel);
		}
	}

	m_pTexture = Locator::videoManager->createTextureFromSurfacePtr(m_windowId, pSurface.get());
	if(m_pTexture == nullptr){
		BOOST_THROW_EXCEPTION(CapEngineException("Could not create texture"));
	}
}

//! \copydoc Widget::handleMouseButtonEvent
void TileCopyControl::handleMouseButtonEvent(SDL_MouseButtonEvent event){

}

//! \copydoc Widget::handleKeyboardEvent
void TileCopyControl::handleKeyboardEvent(SDL_KeyboardEvent event){
	if(event.keysym.sym == SDLK_ESCAPE){
		this->remove();
	}
}

//! removes this Control
void TileCopyControl::remove(){
	boost::any maybeWidgetState = Locator::locate(WidgetState::kWidgetStateLocatorId);
	auto pWidgetState = boost::any_cast<std::shared_ptr<WidgetState>>(maybeWidgetState);
	assert(pWidgetState != nullptr);

	boost::optional<std::shared_ptr<UI::Control>> maybeControl = pWidgetState->peekControl();
	assert(maybeControl);
	assert(this == maybeControl->get());
	pWidgetState->popControl();
}

//! \copydoc Control::setHandledd
void TileCopyControl::setHandled(bool handled){
	if(handled){
		std::shared_ptr<WidgetState> pWidgetState = getWidgetState();
		assert(pWidgetState != nullptr);

		boost::optional<std::shared_ptr<UI::Control>> maybeControl = pWidgetState->peekControl();
		assert(maybeControl != boost::none);
		assert((*maybeControl).get() == this);

		pWidgetState->popControl();
	}
}

}}

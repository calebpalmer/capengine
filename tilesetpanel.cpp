#include "tilesetpanel.h"

#include "locator.h"
#include "VideoManager.h"
#include "scopeguard.h"

namespace CapEngine { namespace UI {

namespace {


//! calculate the numner of columns of tiles can be displayed.
/** 
 \param srcWidth - the width of a tile
 \param dstWidth - the width available for rendering
 \param padding - The amount of padding between each tile
 \return - The number tiles that can be rendered in each row.
*/
int calculateNumColumns(int srcWidth, int dstWidth, int padding){
	int numColumns = dstWidth / srcWidth;
	int numColumnsWithBuffer = numColumns;

	while(numColumnsWithBuffer > 0){
		if(dstWidth / (srcWidth + ((numColumnsWithBuffer + 2) * padding )) == numColumnsWithBuffer){
			return numColumnsWithBuffer;
		}

		numColumnsWithBuffer--;
	}

	return numColumnsWithBuffer;
}

}


//! Constructor
/** 
 \param pTileSet - The tileset.
*/
TileSetPanel::TileSetPanel(std::shared_ptr<TileSet> pTileSet)
	: m_pTileSet(pTileSet), m_pTileSetTexture(getNullTexturePtr())
{
	assert(m_pTileSet != nullptr);
}


//! Creates a new TileSetPanel
/** 
 \param pTileSet - The tileset.
 \return - The new TileSetPanel
*/
std::shared_ptr<TileSetPanel> TileSetPanel::create(std::shared_ptr<TileSet> pTileSet){
	return std::shared_ptr<TileSetPanel>(new TileSetPanel(pTileSet));
}


//! \copydoc TileSetPanel::setPosition
void TileSetPanel::setPosition(int x, int y){
	m_rect.x = x;
	m_rect.y = y;
}


//! \copydoc Widget::setSize
void TileSetPanel::setSize(int width, int height){
	m_rect.w = width;
	m_rect.h = height;
}


//! \copydoc Widget::render
void TileSetPanel::render(){
 	assert(Locator::videoManager != nullptr);

	if(m_pTileSetTexture == nullptr){
		std::shared_ptr<SDL_Surface> pTilesetSurface = m_pTileSet->getSurface();
		assert(pTilesetSurface != nullptr);

		m_pTileSetTexture = Locator::videoManager->createTextureFromSurfacePtr(m_windowId, pTilesetSurface.get());
	}
	
	Locator::videoManager->setClipRect(m_windowId, &m_rect);
	ScopeGuard guard(std::bind(&VideoManager::setClipRect, Locator::videoManager, m_windowId, nullptr));
	
 	int padding = 5;
 	assert(m_pTileSet != nullptr);
	int tileSize = m_pTileSet->getTileSize();
 	int numColumns = calculateNumColumns(tileSize, m_rect.w, padding);

	if(numColumns == 0)
		return;

 	for(size_t i = 0; i < m_pTileSet->getNumTiles(); i++){
 		int row = i / numColumns;
 		int column = i % numColumns;
		
 		Tile tile = m_pTileSet->getTile(i);
		SDL_Rect srcRect = { tile.xpos, tile.ypos, tileSize, tileSize };
		SDL_Rect dstRect = {
			padding + (column * padding) + (column * tileSize) ,
			padding + (row * padding) + (row * tileSize),
			tileSize,
			tileSize
		};

		assert(m_pTileSetTexture != nullptr);
		Locator::videoManager->drawTexture(m_windowId, m_pTileSetTexture.get(), &srcRect, &dstRect);
		
 	}
}

}}

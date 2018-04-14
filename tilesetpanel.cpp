#include "tilesetpanel.h"

#include "locator.h"
#include "VideoManager.h"
#include "scopeguard.h"
#include "collision.h"
#include "tilecopycontrol.h"
#include "editorconstants.h"
#include "widgetstate.h"

#include <boost/numeric/conversion/cast.hpp>

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
		if((srcWidth + ((numColumnsWithBuffer + 2) * padding )) <= dstWidth){
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
	if(m_rect.x != x || m_rect.y != y){
		m_rect.x = x;
		m_rect.y = y;

		m_updateDrawLocations = true;
	}
}


//! \copydoc Widget::setSize
void TileSetPanel::setSize(int width, int height){
	if(m_rect.w != width || m_rect.h != height){
		m_rect.w = width;
		m_rect.h = height;

		m_updateDrawLocations = true;
	}
}


//! \copydoc Widget::render
void TileSetPanel::render(){
 	assert(Locator::videoManager != nullptr);

	if(m_pTileSetTexture == nullptr){
		std::shared_ptr<SDL_Surface> pTilesetSurface = m_pTileSet->getSurface();
		assert(pTilesetSurface != nullptr);

		m_pTileSetTexture = Locator::videoManager->createTextureFromSurfacePtr(m_windowId, pTilesetSurface.get());
	}

	if(m_updateDrawLocations)
		updateDrawLocations();
	
	Locator::videoManager->setClipRect(m_windowId, &m_rect);
	ScopeGuard guard(std::bind(&VideoManager::setClipRect, Locator::videoManager, m_windowId, nullptr));


	for(auto&& tileLocation : m_tiles){
		Locator::videoManager->drawTexture(m_windowId, m_pTileSetTexture.get(), &(tileLocation.srcRect), &(tileLocation.dstRect));		
	}
}

//!  Updates the cached draw location of the tiles
void TileSetPanel::updateDrawLocations(){
 	int padding = 5;
 	assert(m_pTileSet != nullptr);
	int tileSize = m_pTileSet->getTileSize();
 	int numColumns = calculateNumColumns(tileSize, m_rect.w, padding);

	if(numColumns == 0)
		return;

	m_tiles.clear();
 	for(size_t i = 0; i < m_pTileSet->getNumTiles(); i++){
 		int row = i / numColumns;
 		int column = i % numColumns;
		
		CapEngine::Tile tile = m_pTileSet->getTile(i);
		SDL_Rect srcRect = { tile.xpos, tile.ypos, tileSize, tileSize };
		SDL_Rect dstRect = {
			padding + (column * padding) + (column * tileSize) ,
			padding + (row * padding) + (row * tileSize),
			tileSize,
			tileSize
		};

		m_tiles.push_back({boost::numeric_cast<unsigned int>(i), srcRect, dstRect});
	}

	m_updateDrawLocations = false;
}

//! \copydoc Widget::handleMouseButtonEvent
void TileSetPanel::handleMouseButtonEvent(SDL_MouseButtonEvent event){

	if(event.type == SDL_MOUSEBUTTONDOWN){
		int x = event.x;
		int y = event.y;

		for(auto && tileLocation : m_tiles){
			if(pointInRect(Point({x, y}), tileLocation.dstRect)){
				auto pTileCopyControl = std::make_shared<TileCopyControl>(m_pTileSet, tileLocation.index);
				pTileCopyControl->setWindowId(this->m_windowId);
				pTileCopyControl->setParent(this->m_pParent);
				
				boost::any maybeControlStack = Locator::locate(WidgetState::kControlStackId);

				std::shared_ptr<std::vector<std::shared_ptr<UI::Control>>> pControlStack;
				try{
					pControlStack = boost::any_cast<std::shared_ptr<std::vector<std::shared_ptr<UI::Control>>>>(maybeControlStack);
				}
				catch(const boost::bad_any_cast &e){
					assert(Locator::logger != nullptr);
					Locator::logger->log(e, Logger::CWARNING, __FILE__, __LINE__);
					BOOST_THROW_EXCEPTION(CapEngineException("Could not locate control stack"));
				}

				assert(pControlStack != nullptr);
				pControlStack->push_back(pTileCopyControl);
				
				break;
			}
		}
	}
}

}}

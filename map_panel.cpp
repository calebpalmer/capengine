#include "map_panel.h"

#include "locator.h"
#include "logger.h"
#include "CapEngineException.h"
#include "colour.h"
#include "editorconstants.h"
#include "captypes.h"
#include "colour.h"

#include <sstream>
#include <iostream>
#include <cassert>

namespace {
const CapEngine::Colour outlineColour = {91, 110, 225, 255};
}

namespace CapEngine {

MapPanel::MapPanel(Uint32 windowID, bool ownsWindow, int x, int y, int width, int height, std::shared_ptr<Map2D> pMap)
  : m_windowID(windowID), m_x(x), m_y(y), m_width(width), m_height(height)
  , m_pMap(pMap), m_ownsWindow(true)
{
  CAP_THROW_ASSERT(m_pMap.get() != nullptr, "Passed Map2D is null");
}


/**
 resizes the panel 
 */
void MapPanel::resize(int x, int y, int w, int h){
  m_x = x;
  m_y = y;
  m_width = w;
  m_height = h;
}

/**
 * Render the map panel
 */
void MapPanel::render()
{
  auto&& videoManager = Locator::videoManager;
  
  // fill background colous
  videoManager->clearScreen(m_windowID);
  Rect fillRect = { 0, 0, m_width, m_height };
  videoManager->drawFillRect(m_windowID, fillRect, kBackgroundColour);
  
  Surface* surface  = m_pMap->getSurface();
  if(surface != nullptr){
    TexturePtr texture = textureToTexturePtr(videoManager->createTextureFromSurface(m_windowID, surface, false));
  
    int mapWidth = m_pMap->getWidth();
    int mapHeight = m_pMap->getHeight();
    Rect srcRect = {0, 0, 0, 0};
    Rect dstRect = {0, 0, 0, 0};
    

    float mapAspectRatio = static_cast<float>(mapWidth) / static_cast<float>(mapHeight);
    float panelAspectRatio = static_cast<float>(m_width) / static_cast<float>(m_height);

    // TODO this all needs to be revisted so that all cases are handled
    // eg:  if panel is bigger than map, we stretch the map and maintain aspect ratio
    if(mapAspectRatio >= 1  && mapWidth < m_width){
      // stretch width
      srcRect.w = mapWidth;
      srcRect.h = mapHeight;

      // panel is wide screen
      if(panelAspectRatio >= 1){
	dstRect.h = m_height;
	m_scaleFactor = static_cast<float>(m_height) / static_cast<float>(mapHeight);
	dstRect.w = static_cast<float>(mapWidth) * m_scaleFactor;
      }
      // panel is portrait
      else{
	dstRect.w = m_width;
	m_scaleFactor = m_width / mapWidth;
	dstRect.h = mapHeight * m_scaleFactor;
      }
      
      //dstRect.h = m_height;
    }
    
    else if(mapAspectRatio < 1 && mapHeight < m_height){
      srcRect.h = mapHeight;
      srcRect.w = mapWidth;

      dstRect.h = m_height;
      //dstRect.w = m_width;
      dstRect.w = m_height * mapWidth / mapHeight;
    }

    // panel smaller than map.
    else{
      assert(false);
    }

    Locator::videoManager->drawTexture(m_windowID, texture.get(), &srcRect, &dstRect, false);
    this->drawTileOutline();
  }

  else{
    std::ostringstream msg;
    msg << "Map texture is null";
    Locator::logger->log(msg.str(), Logger::CWARNING);
  }

}

void MapPanel::handleMouseMotionEvent(SDL_MouseMotionEvent event){
  if(event.windowID == m_windowID){
    if(m_ownsWindow){
      if(event.x < m_pMap->getWidth() && event.y < m_pMap->getHeight()){
	// we're hovering over a tile
	
	int scaledTileSize = static_cast<double>(m_pMap->getTileSize()) * m_scaleFactor;	
	int xTile = event.x / scaledTileSize;
	int yTile = event.y / scaledTileSize;
	m_hoveredTile = { xTile, yTile };
      }
      else{
	// we're not hovering over a tile
	m_hoveredTile = { -1, -1 };
      }
    }
    else{
      BOOST_THROW_EXCEPTION(CapEngineException("MapPanel does not yet support sharing windows"));
    }
  }
}

void MapPanel::drawTileOutline(){
  if(m_hoveredTile.first != -1 && m_hoveredTile.second != -1){
    CAP_THROW_ASSERT(Locator::videoManager != nullptr,
		     "VideoManager is null");

    int scaledTileSize = static_cast<double>(m_pMap->getTileSize()) * m_scaleFactor;
    Rect dstRect = {0, 0, scaledTileSize, scaledTileSize};
    dstRect.x = m_hoveredTile.first * scaledTileSize;
    dstRect.y = m_hoveredTile.second * scaledTileSize;
    Locator::videoManager->drawRect(m_windowID, dstRect, outlineColour);
  }
}
  
}


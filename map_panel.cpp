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
const CapEngine::Colour selectedColour = {138, 111, 48, 255};
const CapEngine::Colour mouseDragColour = {100, 100, 100, 255};
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
    this->drawSelectedTileOutlines();
    this->drawHoveredTileOutline();
    this->drawMouseDrag();
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
      m_hoveredTile = getHoveredTile(event.x, event.y);
    }

    else{
      BOOST_THROW_EXCEPTION(CapEngineException("MapPanel does not yet support sharing windows"));
    }
  }
}

void MapPanel::handleMouseButtonEvent(SDL_MouseButtonEvent event){
  CAP_THROW_ASSERT(m_ownsWindow, "MapPanel does not yet support sharing windows");
  if(event.windowID == m_windowID){
    if(event.type == SDL_MOUSEBUTTONUP){
      // Mouse button Up
      if((m_dragStart.first == -1 || m_dragStart.second == -1)
	 || (m_dragStart.first == event.x && m_dragStart.second == event.y)){
	// we're not dragging
	std::pair<int, int> selectedTile = getHoveredTile(event.x, event.y);
	const Uint8 *keyboardState = SDL_GetKeyboardState(nullptr);
	CAP_THROW_ASSERT(keyboardState != nullptr, "SDL Keyboard state returned null");
      
	bool addKeyPressed = keyboardState[SDL_SCANCODE_LCTRL] == 1
	  || keyboardState[SDL_SCANCODE_RCTRL] == 1;

	// remove it if it's already selected
	auto searchedTile = std::find(m_selectedTiles.begin(), m_selectedTiles.end(), selectedTile);
	if(searchedTile != m_selectedTiles.end())
	  m_selectedTiles.erase(searchedTile);
	// insert it otherwise
	else{
	  if(!addKeyPressed)
	    m_selectedTiles.clear();
	  m_selectedTiles.push_back(selectedTile);
	}

	m_dragStart = {-1, -1};
      }
      else{
	// we're dragging
	m_selectedTiles.clear();
	auto index1 = getHoveredTile(m_dragStart.first, m_dragStart.second);
	auto index2 = getHoveredTile(event.x, event.y);
	m_dragStart = {-1, -1};
	
	auto && leftIndex = index1.first < index2.first ? index1 : index2;
	auto && rightIndex = index1.first > index2.first ? index1 : index2;

	for(int i = leftIndex.first; i <= rightIndex.first; i++){
	  int j = leftIndex.second;
	  if(j <= rightIndex.second){
	    for(; j <= rightIndex.second; j++)
	      m_selectedTiles.push_back(std::make_pair(i, j));
	  }
	  else{
	    for(; j >= rightIndex.second; j--)
	      m_selectedTiles.push_back(std::make_pair(i, j));
	  }
	}
      }
    }

    else{
      // Mouse button down
      m_dragStart = {event.x, event.y};
    }
    
  }
  
}

void MapPanel::drawHoveredTileOutline(){
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

void MapPanel::drawSelectedTileOutlines(){
  for(auto && tileIndex : m_selectedTiles){
    CAP_THROW_ASSERT(Locator::videoManager != nullptr,
		     "VideoManager is null");
    
    int scaledTileSize = static_cast<double>(m_pMap->getTileSize()) * m_scaleFactor;
    Rect dstRect = {0, 0, scaledTileSize, scaledTileSize};
    dstRect.x = tileIndex.first * scaledTileSize;
    dstRect.y = tileIndex.second * scaledTileSize;
    Locator::videoManager->drawRect(m_windowID, dstRect, selectedColour);
  }
}

void MapPanel::drawMouseDrag(){
  CAP_THROW_ASSERT(Locator::videoManager != nullptr,
		   "VideoManager is null");
  
  if(m_dragStart.first != -1 && m_dragStart.second != -1){
    int x = -1;
    int y = -1;
    SDL_GetMouseState(&x, &y);
    Rect rect = {m_dragStart.first, m_dragStart.second,
		 x - m_dragStart.first,
		 y - m_dragStart.second};

    Locator::videoManager->drawRect(m_windowID, rect, mouseDragColour);
    }
}

std::pair<int, int> MapPanel::getHoveredTile(int x, int y) const{
  if(x < m_pMap->getWidth() * m_scaleFactor && y < m_pMap->getHeight() * m_scaleFactor){  
    int scaledTileSize = static_cast<double>(m_pMap->getTileSize()) * m_scaleFactor;	
    int xTile = x / scaledTileSize;
    int yTile = y / scaledTileSize;
    return {xTile, yTile};
  }
  else{
    return {-1, -1};
  }
  
}
  
}


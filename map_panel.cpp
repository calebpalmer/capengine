#include "map_panel.h"

#include "locator.h"
#include "logger.h"
#include "CapEngineException.h"
#include "colour.h"
#include "editorconstants.h"
#include "captypes.h"
#include "colour.h"
#include "utils.h"
#include "scopeguard.h"
#include "editorutils.h"
#include "control.h"
#include "tilecopycontrol.h"

#include <sstream>
#include <iostream>
#include <cassert>
#include <cmath>

namespace CapEngine { namespace UI {

namespace {

const CapEngine::Colour outlineColour = {91, 110, 225, 255};
const CapEngine::Colour selectedColour = {138, 111, 48, 255};
const CapEngine::Colour mouseDragColour = {100, 100, 100, 255};

const float kScaleIncrement = 0.1;
const float kTranslationIncrement = 4.0;

void scaleMatrix(CapEngine::Matrix& matrix, float scaleIncrement){
  matrix = matrix + CapEngine::Matrix::createScaleMatrix(scaleIncrement, scaleIncrement, scaleIncrement);
}

}


//! Constructor
/** 
 \param pMap - The map to show on the panel.
*/
MapPanel::MapPanel(std::shared_ptr<Map2D> pMap)
  : m_pMap(pMap)
{
  CAP_THROW_ASSERT(m_pMap.get() != nullptr, "Passed Map2D is null");
}



//! creates a MapPanel
/** 
 \param pMap - The map the panel holds.
 \return - The mapPanel
*/
std::shared_ptr<MapPanel> MapPanel::create(std::shared_ptr<Map2D> pMap){
	return std::shared_ptr<MapPanel>(new MapPanel(pMap));
}

//! Called when performing initial location initialization
void MapPanel::doLocationInit(){
  // set initial scale size
	assert(m_pMap != nullptr);
	
	int mapWidth = m_pMap->getWidth();
	int mapHeight = m_pMap->getHeight();

	float scaleFactor;

	int buffer = 10;
	
	SDL_Rect srcRect = { m_x, m_y, mapWidth, mapHeight };
	SDL_Rect dstRect = { m_x + buffer, m_y + buffer, m_width - buffer, m_height - buffer};
	SDL_Rect scaledRect = expandRectToFit(srcRect, dstRect);

	// set scaling
	scaleFactor = static_cast<float>(scaledRect.w) / static_cast<float>(mapWidth);

	m_scaleMatrix = m_scaleMatrix * Matrix::createScaleMatrix(scaleFactor, scaleFactor, scaleFactor);

	// set translation (CapEngine::expandToFit() centers it)
	m_translationMatrix = m_translationMatrix *
		Matrix::createTranslationMatrix(static_cast<double>(scaledRect.x) - static_cast<double>(dstRect.x) + static_cast<double>(buffer),
																		static_cast<double>(scaledRect.y) - static_cast<double>(dstRect.y) + static_cast<double>(buffer),
																		0.0);

	m_locationInitialized = true;
}

//! \copydoc Widget::setPosition
void MapPanel::setPosition(int x, int y){
	m_x = x;
	m_y = y;
}

//! \copydoc Widget::setSize
void MapPanel::setSize(int width, int height){
	m_width = width;
	m_height = height;
}

//! \copydoc Widget::update
void MapPanel::update(double /* ms */){
}

//! \copydoc Widget::render
void MapPanel::render()
{
  VideoManager *videoManager = Locator::videoManager;
	assert(videoManager != nullptr);

	if(!m_locationInitialized)
		doLocationInit();

  Surface* surface  = m_pMap->getSurface();
  if(surface != nullptr){
    TexturePtr texture = textureToTexturePtr(videoManager->createTextureFromSurface(m_windowId, surface, false));
  
    int mapWidth = m_pMap->getWidth();
    int mapHeight = m_pMap->getHeight();
    Rect srcRect = {0, 0, 0, 0};
    Rect dstRect = {m_x, m_y, 0, 0};

    srcRect.w = mapWidth;
    srcRect.h = mapHeight;

    Vector dstVectorOrigin(static_cast<double>(m_x), static_cast<double>(m_y), 0.0, 1.0);
    dstVectorOrigin = m_translationMatrix * dstVectorOrigin;

    Vector dstVectorDims(mapWidth, mapHeight, 0.0, 1.0);
    dstVectorDims = m_scaleMatrix * dstVectorDims;

    dstRect.x = dstVectorOrigin.getX();
    dstRect.y = dstVectorOrigin.getY();
    dstRect.w = dstVectorDims.getX();
    dstRect.h = dstVectorDims.getY();

		// clip to current rect
		SDL_Rect clipRect({m_x, m_y, m_width, m_height});
		ScopeGuard guard([&]() { videoManager->setClipRect(m_windowId, nullptr); });
		videoManager->setClipRect(m_windowId, &clipRect);
		
    Locator::videoManager->drawTexture(m_windowId, texture.get(), &srcRect, &dstRect, false);
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

//! \copydoc Widget::handleMouseMotionevent
void MapPanel::handleMouseMotionEvent(SDL_MouseMotionEvent event){
	std::shared_ptr<WidgetState> pWidgetState = getWidgetState();
	assert(pWidgetState != nullptr);
	
	boost::optional<std::shared_ptr<UI::Control>> maybeControl = pWidgetState->peekControl();
	if(maybeControl != boost::none){
		auto pTileCopyControl = std::dynamic_pointer_cast<TileCopyControl>(*maybeControl);
		if(pTileCopyControl){
			// TODO change the tile here
		}
	}
	else{
		if(m_dragState == DRAGSTATE_PAN){
			Vector translationVector = Vector(event.x, event.y, 0.0, 1.0) -
				Vector(m_lastMotionLocation.first, m_lastMotionLocation.second, 0.0, 1.0);;

			// m_translationMatrix = m_translationMatrix +
			// 	Matrix::createTranslationMatrix(translationVector.getX(), translationVector.getY(), translationVector.getZ());

		}

		m_lastMotionLocation = {event.x, event.y};
	}
}

//! \copydoc Widget::handleMouseButtonEvent
void MapPanel::handleMouseButtonEvent(SDL_MouseButtonEvent event){

	if(event.type == SDL_MOUSEBUTTONUP && event.button == SDL_BUTTON_LEFT){
		handleLeftMouseButtonUp(event);
	}

	else if(event.type == SDL_MOUSEBUTTONUP && event.button == SDL_BUTTON_MIDDLE){
		handleMiddleMouseButtonUp(event);
	}

	else if(event.type == SDL_MOUSEBUTTONDOWN && event.button == SDL_BUTTON_LEFT){
		handleLeftMouseButtonDown(event);
	}

	else if(event.type == SDL_MOUSEBUTTONDOWN && event.button == SDL_BUTTON_MIDDLE){
		handleMiddleMouseButtonDown(event);
	}
	
}

void MapPanel::handleLeftMouseButtonUp(SDL_MouseButtonEvent event){

  // Mouse button Up
  if((m_dragState == DRAGSTATE_NONE) ||
     (m_dragStart.first == event.x && m_dragStart.second == event.y)){
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
		m_dragState = DRAGSTATE_NONE;
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

void MapPanel::handleLeftMouseButtonDown(SDL_MouseButtonEvent event){
  if(isInMap(event.x, event.y)){
    if(m_dragState == DRAGSTATE_NONE){
      m_dragState = DRAGSTATE_SELECT;
      m_dragStart = {event.x, event.y};
    }

  }
}

void MapPanel::handleMiddleMouseButtonUp(SDL_MouseButtonEvent event){
  if(m_dragState == DRAGSTATE_PAN){
    m_dragState = DRAGSTATE_NONE;
  }
}

void MapPanel::handleMiddleMouseButtonDown(SDL_MouseButtonEvent event){
  if(isInMap(event.x, event.y)){
    if(event.button == SDL_BUTTON_MIDDLE && m_dragState == DRAGSTATE_NONE){
      m_dragState = DRAGSTATE_PAN;
      m_dragStart = {event.x, event.y};
			m_lastMotionLocation = {event.x, event.y};
    }
  }
}


void MapPanel::handleKeyboardEvent(SDL_KeyboardEvent event){

  if(event.type == SDL_KEYDOWN){
    switch(event.keysym.sym){
    case SDLK_EQUALS:
      {
    const Uint8 *keyboardState = SDL_GetKeyboardState(nullptr);
    CAP_THROW_ASSERT(keyboardState != nullptr, "SDL Keyboard state returned null");

	if(keyboardState[SDL_SCANCODE_LSHIFT] == 1
	   || keyboardState[SDL_SCANCODE_RSHIFT] == 1)
	  scaleMatrix(m_scaleMatrix, kScaleIncrement);
      }
      break;
    case SDLK_MINUS:
      scaleMatrix(m_scaleMatrix, (-1) * kScaleIncrement);
      break;

    case SDLK_RIGHT:
      m_translationMatrix = m_translationMatrix * Matrix::createTranslationMatrix((-1.0) * kTranslationIncrement, 0.0, 0.0);
			// TODO fix this
      //m_translationMatrix.getRowVectorRef(3).setD(1.0);
      break;
    case SDLK_LEFT:
      m_translationMatrix = m_translationMatrix * Matrix::createTranslationMatrix(kTranslationIncrement, 0.0, 0.0);
			// TODO fix this
      //m_translationMatrix.getRowVectorRef(3).setD(1.0);      
      break;
    case SDLK_UP:
      m_translationMatrix = m_translationMatrix * Matrix::createTranslationMatrix(0.0, kTranslationIncrement, 0.0);
			// TODO fix this
      //m_translationMatrix.getRowVectorRef(3).setD(1.0);      
      break;
    case SDLK_DOWN:
      m_translationMatrix = m_translationMatrix * Matrix::createTranslationMatrix(0.0, (-1.0) * kTranslationIncrement, 0.0);
			// TODO fix ths
      //m_translationMatrix.getRowVectorRef(3).setD(1.0);      
      break;
    default:
      break;
    }

  }
}

void MapPanel::handleMouseWheelEvent(SDL_MouseWheelEvent event){
  scaleMatrix(m_scaleMatrix, event.y * kScaleIncrement);
}

//! Draws outlines around given tiles in the give colour
/**
* \param tiles The tiles to draw outlines around
* \param colour The colour to draw the outline with
*/
void MapPanel::drawTileOutlines(const std::vector<std::pair<int, int>> &tiles,
																CapEngine::Colour colour){

	CAP_THROW_ASSERT(Locator::videoManager != nullptr,
									 "VideoManager is null");

  for(auto && tile : tiles){
		
		Vector mapOrigin = m_translationMatrix * Vector(static_cast<double>(m_x), static_cast<double>(m_y), 0.0, 1.0);
		double scaledTileSize = this->getScaledTileSize();

    Rect dstRect = {0, 0,
										static_cast<int>(round(scaledTileSize)),
										static_cast<int>(round(scaledTileSize))};
    dstRect.x = round(mapOrigin.getX() + (static_cast<double>(tile.first) * scaledTileSize));
    dstRect.y = round(mapOrigin.getY() + (static_cast<double>(tile.second) * scaledTileSize));

    Locator::videoManager->drawRect(m_windowId, dstRect, colour);
	}
}

//! Draws an outline over any map tile that the mouse is on top of
void MapPanel::drawHoveredTileOutline(){
	int x = 0;
	int y = 0;
	SDL_GetMouseState(&x, &y);
	
	std::pair<int, int> hoveredTile = getHoveredTile(x, y);
  if(hoveredTile.first != -1 && hoveredTile.second != -1){
		std::vector<std::pair<int,int>> tiles(1);
		tiles[0] = hoveredTile;
		drawTileOutlines(tiles, outlineColour);
	}
}

void MapPanel::drawSelectedTileOutlines(){
	drawTileOutlines(m_selectedTiles, selectedColour);
}

void MapPanel::drawMouseDrag(){
  CAP_THROW_ASSERT(Locator::videoManager != nullptr,
		   "VideoManager is null");
  
  if(m_dragState == DRAGSTATE_SELECT && m_dragStart.first != -1 && m_dragStart.second != -1){
    int x = -1;
    int y = -1;
    SDL_GetMouseState(&x, &y);
    Rect rect = {m_dragStart.first, m_dragStart.second,
		 x - m_dragStart.first,
		 y - m_dragStart.second};

    Locator::videoManager->drawRect(m_windowId, rect, mouseDragColour);
    }
}

std::pair<int, int> MapPanel::getHoveredTile(int x, int y) const{
  if(isInMap(x, y))
  {
		double scaledTileSize = this->getScaledTileSize();
    Vector mapOrigin = m_translationMatrix * Vector(static_cast<double>(m_x), static_cast<double>(m_y), 0.0, 1.0);

    int xTile = (x - mapOrigin.getX()) / scaledTileSize;
    int yTile = (y - mapOrigin.getY()) / scaledTileSize;
    return {xTile, yTile};
  }
  else{
    return {-1, -1};
  }
  
}


//! checks if the location in window coordinates is in the map.
/** 
 \param 
 \param 
 \return 
*/
bool MapPanel::isInMap(int x, int y) const{
	SDL_Rect mapExtents = this->getVisibleMapExtents();

  return x >= mapExtents.x && x < mapExtents.x + mapExtents.w &&
				 y >= mapExtents.y && y < mapExtents.y + mapExtents.h;
}

//! Gets the location and dimensions of the map within the window
/** 
 \return - The SDL_Rect representing its location and dimensions.
*/
SDL_Rect MapPanel::getMapExtents() const{

	Vector mapOrigin = m_translationMatrix * Vector(static_cast<double>(m_x), static_cast<double>(m_y), 0.0, 1.0);
  Vector mapDims = m_scaleMatrix * Vector(m_pMap->getWidth(), m_pMap->getHeight(), 0.0, 1.0);

	return { static_cast<int>(mapOrigin.getX()),
			     static_cast<int>(mapOrigin.getY()),
			     static_cast<int>(mapDims.getX()),
			     static_cast<int>(mapDims.getY())
			};
	
}

//! Gets the visible extents of the map
/** 
 \return - The SDL_Rect representing the visible extents of the map
*/
SDL_Rect MapPanel::getVisibleMapExtents() const{
	SDL_Rect mapExtents = this->getMapExtents();
	SDL_Rect drawExtents = {m_x, m_y, m_width, m_height};

	std::optional<SDL_Rect> maybeVisibleMapExtents = intersectRects(mapExtents, drawExtents);

	if(maybeVisibleMapExtents != std::nullopt)
		return *maybeVisibleMapExtents;

	return {0, 0, 0, 0};
}


//! Gets the current scaled display size of tiles in the map.
/** 
 \return The scaled tile size
*/
double MapPanel::getScaledTileSize() const{
  real scaleFactor = m_scaleMatrix.getRowVector(0).getX();
	return static_cast<double>(m_pMap->getTileSize()) * scaleFactor;	
}
  
}}


#include "map_panel.h"

#include "CapEngineException.h"
#include "aggregatecommand.h"
#include "captypes.h"
#include "colour.h"
#include "control.h"
#include "editor.h"
#include "editorconstants.h"
#include "locator.h"
#include "logger.h"
#include "pancontrol.h"
#include "scopeguard.h"
#include "simplecommand.h"
#include "tilecopycontrol.h"
#include "uiutils.h"
#include "utils.h"

#include <boost/polymorphic_pointer_cast.hpp>
#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>

namespace CapEngine
{
namespace UI
{

namespace
{

const CapEngine::Colour outlineColour = {91, 110, 225, 255};
const CapEngine::Colour selectedColour = {138, 111, 48, 255};
const CapEngine::Colour mouseDragColour = {100, 100, 100, 255};

const float kScaleIncrement = 0.1;
const float kTranslationIncrement = 4.0;

void scaleMatrix(CapEngine::Matrix &matrix, float scaleIncrement)
{
  matrix = matrix + CapEngine::Matrix::createScaleMatrix(
						scaleIncrement, scaleIncrement, scaleIncrement);
}

//! Creates a command for setting a tile in a map.
/**
 \param pMap
   \li The map to change.
 \param x
   \li The x index into the map.
 \param y
   \li The y index into the map.
 \param tileIndex
   \li The index into the tileset to set.
 \return
*/
std::unique_ptr<Command> makeSetTileCommand(std::shared_ptr<Map2D> pMap, int x,
											int y, int tileIndex)
{
  auto execute = [pMap, x, y, tileIndex]() { pMap->setTile(x, y, tileIndex); };

  int oldTileIndex = pMap->getTileIndex(x, y);
  auto unExecute = [pMap, x, y, oldTileIndex]() {
	pMap->setTile(x, y, oldTileIndex);
  };

  return std::make_unique<SimpleCommand>(execute, unExecute);
}

} // namespace

//! Constructor
/**
 \param pMap - The map to show on the panel.
*/
MapPanel::MapPanel(std::shared_ptr<Map2D> pMap) : m_pMap(pMap)
{
  CAP_THROW_ASSERT(m_pMap.get() != nullptr, "Passed Map2D is null");
}

//! creates a MapPanel
/**
 \param pMap - The map the panel holds.
 \return - The mapPanel
*/
std::shared_ptr<MapPanel> MapPanel::create(std::shared_ptr<Map2D> pMap)
{
  return std::shared_ptr<MapPanel>(new MapPanel(pMap));
}

//! Called when performing initial location initialization
void MapPanel::doLocationInit()
{
  // set initial scale size
  assert(m_pMap != nullptr);

  int mapWidth = m_pMap->getWidth();
  int mapHeight = m_pMap->getHeight();

  float scaleFactor;

  int buffer = 10;

  SDL_Rect srcRect = {m_x, m_y, mapWidth, mapHeight};
  SDL_Rect dstRect = {m_x + buffer, m_y + buffer, m_width - buffer,
					  m_height - buffer};
  SDL_Rect scaledRect = expandRectToFit(srcRect, dstRect);

  // set scaling
  scaleFactor = static_cast<float>(scaledRect.w) / static_cast<float>(mapWidth);

  m_scaleMatrix = m_scaleMatrix * Matrix::createScaleMatrix(
									  scaleFactor, scaleFactor, scaleFactor);

  // set translation (CapEngine::expandToFit() centers it)
  m_translationMatrix =
	  m_translationMatrix *
	  Matrix::createTranslationMatrix(
		  static_cast<double>(scaledRect.x) - static_cast<double>(dstRect.x) +
			  static_cast<double>(buffer),
		  static_cast<double>(scaledRect.y) - static_cast<double>(dstRect.y) +
			  static_cast<double>(buffer),
		  0.0);

  m_locationInitialized = true;
}

//! \copydoc Widget::setPosition
void MapPanel::setPosition(int x, int y)
{
  m_x = x;
  m_y = y;
}

//! \copydoc Widget::setSize
void MapPanel::setSize(int width, int height)
{
  m_width = width;
  m_height = height;
}

//! \copydoc Widget::update
void MapPanel::update(double /* ms */) {}

//! \copydoc Widget::render
void MapPanel::render()
{
  VideoManager *videoManager = Locator::videoManager;
  assert(videoManager != nullptr);

  if (!m_locationInitialized)
	doLocationInit();

  Surface *surface = m_pMap->getSurface();
  if (surface != nullptr) {
	TexturePtr texture = textureToTexturePtr(
		videoManager->createTextureFromSurface(m_windowId, surface, false));

	int mapWidth = m_pMap->getWidth();
	int mapHeight = m_pMap->getHeight();
	Rect srcRect = {0, 0, 0, 0};
	Rect dstRect = {m_x, m_y, 0, 0};

	srcRect.w = mapWidth;
	srcRect.h = mapHeight;

	Vector dstVectorOrigin(static_cast<double>(m_x), static_cast<double>(m_y),
						   0.0, 1.0);
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

	Locator::videoManager->drawTexture(m_windowId, texture.get(), &srcRect,
									   &dstRect, false);

	// draw outlines over tiles (drag?)
	this->drawTileOutlines(m_outlinedTiles, Colour(0, 0, 0, 255));
  }

  else {
	std::ostringstream msg;
	msg << "Map texture is null";
	Locator::logger->log(msg.str(), Logger::CWARNING);
  }
}

//! \copydoc Widget::handleMouseMotionevent
void MapPanel::handleMouseMotionEvent(SDL_MouseMotionEvent event)
{

  boost::optional<std::shared_ptr<Control>> maybeControl = getCurrentControl();
  if (maybeControl != boost::none) {

	// pan control?
	auto pPanControl = std::dynamic_pointer_cast<PanControl>(*maybeControl);
	if (pPanControl != nullptr) {
	  int x, y;
	  SDL_GetMouseState(&x, &y);

	  int old_x, old_y;
	  std::tie(old_x, old_y) = pPanControl->getLocation();

	  m_translationMatrix =
		  m_translationMatrix *
		  Matrix::createTranslationMatrix(x - old_x, y - old_y, 0);
	  pPanControl->setLocation(x, y);
	}

	//? tile copy control?
	auto pTileCopyControl =
		std::dynamic_pointer_cast<TileCopyControl>(*maybeControl);
	if (pTileCopyControl != nullptr) {
	  if (pTileCopyControl->isDragging()) {
		int x = -1;
		int y = -1;
		SDL_GetMouseState(&x, &y);

		boost::optional maybeInitialCoords =
			pTileCopyControl->getInitialCoords();
		assert(maybeInitialCoords != boost::none);

		Rect dragRect = {maybeInitialCoords->first, maybeInitialCoords->second,
						 x - maybeInitialCoords->first,
						 y - maybeInitialCoords->second};

		m_outlinedTiles = this->getTilesInRect(dragRect);
	  }
	}
  }
}

//! \copydoc Widget::handleMouseButtonEvent
void MapPanel::handleMouseButtonEvent(SDL_MouseButtonEvent event)
{

  if (event.type == SDL_MOUSEBUTTONUP && event.button == SDL_BUTTON_LEFT) {
	handleLeftMouseButtonUp(event);
  }

  else if (event.type == SDL_MOUSEBUTTONUP &&
		   event.button == SDL_BUTTON_MIDDLE) {
	handleMiddleMouseButtonUp(event);
  }

  else if (event.type == SDL_MOUSEBUTTONDOWN &&
		   event.button == SDL_BUTTON_LEFT) {
	handleLeftMouseButtonDown(event);
  }

  else if (event.type == SDL_MOUSEBUTTONDOWN &&
		   event.button == SDL_BUTTON_MIDDLE) {
	handleMiddleMouseButtonDown(event);
  }
}

void MapPanel::handleLeftMouseButtonUp(SDL_MouseButtonEvent event)
{

  int x = 0;
  int y = 0;
  SDL_GetMouseState(&x, &y);
  if (!pointInRect({x, y}, {m_x, m_y, m_width, m_height}))
	return;

  std::shared_ptr<Editor> pEditor =
	  std::dynamic_pointer_cast<Editor>(getWidgetState());
  assert(pEditor != nullptr);

  boost::optional<std::shared_ptr<UI::Control>> maybeControl =
	  getCurrentControl();

  if (maybeControl != boost::none) {

	// tile copy control
	auto pTileCopyControl =
		std::dynamic_pointer_cast<TileCopyControl>(*maybeControl);
	if (pTileCopyControl != nullptr) {

	  // not dragging, edit individual location
	  if (!pTileCopyControl->isDragging()) {
		// find the current loccation in the map
		std::pair<int, int> hoveredTile = getHoveredTile(x, y);
		if (hoveredTile.first != -1 && hoveredTile.second != -1) {
		  std::unique_ptr<Command> pSetTileCommand =
			  makeSetTileCommand(m_pMap, hoveredTile.first, hoveredTile.second,
								 pTileCopyControl->getIndex());
		  assert(pSetTileCommand != nullptr);

		  pEditor->getCommandManager().runCommand(std::move(pSetTileCommand));
		}
	  }

	  // dragging, multi location tile copy
	  else {
		int x = -1;
		int y = -1;
		SDL_GetMouseState(&x, &y);

		boost::optional maybeInitialCoords =
			pTileCopyControl->getInitialCoords();
		assert(maybeInitialCoords != boost::none);

		Rect dragRect = {maybeInitialCoords->first, maybeInitialCoords->second,
						 x - maybeInitialCoords->first,
						 y - maybeInitialCoords->second};

		std::vector<std::pair<int, int>> tilesInDrag =
			this->getTilesInRect(dragRect);
		std::vector<std::unique_ptr<Command>> commands;
		commands.reserve(tilesInDrag.size());

		for (auto &&i : tilesInDrag) {
		  std::unique_ptr<Command> pSetTileCommand = makeSetTileCommand(
			  m_pMap, i.first, i.second, pTileCopyControl->getIndex());
		  assert(pSetTileCommand != nullptr);

		  commands.push_back(std::move(pSetTileCommand));
		}

		auto pAggregateCommand =
			std::make_unique<AggregateCommand>(std::move(commands));
		pEditor->getCommandManager().runCommand(std::move(pAggregateCommand));

		m_outlinedTiles.clear();
	  }
	} // end tile copy control
  }
}

void MapPanel::handleLeftMouseButtonDown(SDL_MouseButtonEvent event)
{

  boost::optional<std::shared_ptr<Control>> maybeControl = getCurrentControl();
  if (maybeControl == boost::none) {
	// if there is no control, and cursor is on map, push on a pan control.
	int x, y;
	SDL_GetMouseState(&x, &y);

	if (isInMap(x, y)) {
	  auto pPanControl = std::make_shared<PanControl>(x, y);

	  std::shared_ptr<WidgetState> pWidgetState = getWidgetState();
	  assert(pWidgetState != nullptr);

	  pWidgetState->addControl(pPanControl);
	}
  }
}

void MapPanel::handleMiddleMouseButtonUp(SDL_MouseButtonEvent event) {}

void MapPanel::handleMiddleMouseButtonDown(SDL_MouseButtonEvent event) {}

void MapPanel::handleKeyboardEvent(SDL_KeyboardEvent event)
{

  if (event.type == SDL_KEYDOWN) {
	switch (event.keysym.sym) {
	case SDLK_EQUALS: {
	  const Uint8 *keyboardState = SDL_GetKeyboardState(nullptr);
	  CAP_THROW_ASSERT(keyboardState != nullptr,
					   "SDL Keyboard state returned null");

	  if (keyboardState[SDL_SCANCODE_LSHIFT] == 1 ||
		  keyboardState[SDL_SCANCODE_RSHIFT] == 1)
		scaleMatrix(m_scaleMatrix, kScaleIncrement);
	} break;
	case SDLK_MINUS:
	  scaleMatrix(m_scaleMatrix, (-1) * kScaleIncrement);
	  break;

	case SDLK_RIGHT:
	  m_translationMatrix =
		  m_translationMatrix * Matrix::createTranslationMatrix(
									(-1.0) * kTranslationIncrement, 0.0, 0.0);
	  // TODO fix this
	  // m_translationMatrix.getRowVectorRef(3).setD(1.0);
	  break;
	case SDLK_LEFT:
	  m_translationMatrix =
		  m_translationMatrix *
		  Matrix::createTranslationMatrix(kTranslationIncrement, 0.0, 0.0);
	  // TODO fix this
	  // m_translationMatrix.getRowVectorRef(3).setD(1.0);
	  break;
	case SDLK_UP:
	  m_translationMatrix =
		  m_translationMatrix *
		  Matrix::createTranslationMatrix(0.0, kTranslationIncrement, 0.0);
	  // TODO fix this
	  // m_translationMatrix.getRowVectorRef(3).setD(1.0);
	  break;
	case SDLK_DOWN:
	  m_translationMatrix =
		  m_translationMatrix * Matrix::createTranslationMatrix(
									0.0, (-1.0) * kTranslationIncrement, 0.0);
	  // TODO fix ths
	  // m_translationMatrix.getRowVectorRef(3).setD(1.0);
	  break;
	default:
	  break;
	}
  }
}

void MapPanel::handleMouseWheelEvent(SDL_MouseWheelEvent event)
{
  scaleMatrix(m_scaleMatrix, event.y * kScaleIncrement);
}

//! Draws outlines around given tiles in the give colour
/**
 * \param tiles The tiles to draw outlines around
 * \param colour The colour to draw the outline with
 */
void MapPanel::drawTileOutlines(const std::vector<std::pair<int, int>> &tiles,
								CapEngine::Colour colour)
{

  CAP_THROW_ASSERT(Locator::videoManager != nullptr, "VideoManager is null");

  for (auto &&tile : tiles) {

	Vector mapOrigin =
		m_translationMatrix *
		Vector(static_cast<double>(m_x), static_cast<double>(m_y), 0.0, 1.0);
	double scaledTileSize = this->getScaledTileSize();

	Rect dstRect = {0, 0, static_cast<int>(round(scaledTileSize)),
					static_cast<int>(round(scaledTileSize))};
	dstRect.x = round(mapOrigin.getX() +
					  (static_cast<double>(tile.first) * scaledTileSize));
	dstRect.y = round(mapOrigin.getY() +
					  (static_cast<double>(tile.second) * scaledTileSize));

	Locator::videoManager->drawRect(m_windowId, dstRect, colour);
  }
}

//! Draws an outline over any map tile that the mouse is on top of
void MapPanel::drawHoveredTileOutline()
{
  int x = 0;
  int y = 0;
  SDL_GetMouseState(&x, &y);

  std::pair<int, int> hoveredTile = getHoveredTile(x, y);
  if (hoveredTile.first != -1 && hoveredTile.second != -1) {
	std::vector<std::pair<int, int>> tiles(1);
	tiles[0] = hoveredTile;
	drawTileOutlines(tiles, outlineColour);
  }
}

void MapPanel::drawSelectedTileOutlines()
{
  drawTileOutlines(m_selectedTiles, selectedColour);
}

void MapPanel::drawMouseDrag()
{
  CAP_THROW_ASSERT(Locator::videoManager != nullptr, "VideoManager is null");

  if (m_dragState == DRAGSTATE_SELECT && m_dragStart.first != -1 &&
	  m_dragStart.second != -1) {
	int x = -1;
	int y = -1;
	SDL_GetMouseState(&x, &y);
	Rect rect = {m_dragStart.first, m_dragStart.second, x - m_dragStart.first,
				 y - m_dragStart.second};

	Locator::videoManager->drawRect(m_windowId, rect, mouseDragColour);
  }
}

std::pair<int, int> MapPanel::getHoveredTile(int x, int y) const
{
  if (isInMap(x, y)) {
	double scaledTileSize = this->getScaledTileSize();
	Vector mapOrigin =
		m_translationMatrix *
		Vector(static_cast<double>(m_x), static_cast<double>(m_y), 0.0, 1.0);

	int xTile = (x - mapOrigin.getX()) / scaledTileSize;
	int yTile = (y - mapOrigin.getY()) / scaledTileSize;
	return {xTile, yTile};
  } else {
	return {-1, -1};
  }
}

//! checks if the location in window coordinates is in the map.
/**
 \param
 \param
 \return
*/
bool MapPanel::isInMap(int x, int y) const
{
  SDL_Rect mapExtents = this->getVisibleMapExtents();

  return x > mapExtents.x && x < mapExtents.x + mapExtents.w - 1 &&
		 y > mapExtents.y && y < mapExtents.y + mapExtents.h - 1;
}

//! Gets the location and dimensions of the map within the window
/**
 \return - The SDL_Rect representing its location and dimensions.
*/
SDL_Rect MapPanel::getMapExtents() const
{

  Vector mapOrigin =
	  m_translationMatrix *
	  Vector(static_cast<double>(m_x), static_cast<double>(m_y), 0.0, 1.0);
  Vector mapDims =
	  m_scaleMatrix * Vector(m_pMap->getWidth(), m_pMap->getHeight(), 0.0, 1.0);

  return {static_cast<int>(mapOrigin.getX()),
		  static_cast<int>(mapOrigin.getY()), static_cast<int>(mapDims.getX()),
		  static_cast<int>(mapDims.getY())};
}

//! Gets the visible extents of the map
/**
 \return - The SDL_Rect representing the visible extents of the map
*/
SDL_Rect MapPanel::getVisibleMapExtents() const
{
  SDL_Rect mapExtents = this->getMapExtents();
  SDL_Rect drawExtents = {m_x, m_y, m_width, m_height};

  std::optional<SDL_Rect> maybeVisibleMapExtents =
	  intersectRects(mapExtents, drawExtents);

  if (maybeVisibleMapExtents != std::nullopt)
	return *maybeVisibleMapExtents;

  return {0, 0, 0, 0};
}

//! Gets the current scaled display size of tiles in the map.
/**
 \return The scaled tile size
*/
double MapPanel::getScaledTileSize() const
{
  double scaleFactor = m_scaleMatrix.getRowVector(0).getX();
  return static_cast<double>(m_pMap->getTileSize()) * scaleFactor;
}

//! Get a list of tile rects at the given size and location.
/**
 \return
   \li The list of pairs
		 \li first is if the tile is visible
		 \li second is the rectangle.
*/
std::vector<std::tuple<int, int, bool, SDL_Rect>> MapPanel::getTileRects() const
{
  double scaledTileSize = this->getScaledTileSize();
  SDL_Rect visibleExtents = this->getVisibleMapExtents();
  Vector mapOrigin =
	  m_translationMatrix *
	  Vector(static_cast<double>(m_x), static_cast<double>(m_y), 0.0, 1.0);
  int tileSize = m_pMap->getTileSize();
  int numTilesWide = m_pMap->getWidth() / tileSize;
  int numTilesHigh = m_pMap->getHeight() / tileSize;

  std::vector<std::tuple<int, int, bool, SDL_Rect>> rects;
  for (int x = 0; x < numTilesWide; x++) {
	for (int y = 0; y < numTilesHigh; y++) {
	  SDL_Rect rect;
	  rect.w = scaledTileSize;
	  rect.h = scaledTileSize;

	  Vector location(mapOrigin.getX() + (x * scaledTileSize),
					  mapOrigin.getY() + (y * scaledTileSize), 0, 1);

	  rect.x = location.getX();
	  rect.y = location.getY();

	  bool visible = detectMBRCollision(rect, visibleExtents) != COLLISION_NONE;

	  rects.emplace_back(std::make_tuple(x, numTilesHigh - y, visible, rect));
	}
  }

  return rects;
}

//! Gets all tiles that are in or intersect with the given rect.
/**
 \param rect
   \li The rect to test intersection with.
 \return
   \li The list off tiles in the form of pairs or x,y indexes into the map.
*/
std::vector<std::pair<int, int>>
	MapPanel::getTilesInRect(const SDL_Rect &rect) const
{
  std::vector<std::pair<int, int>> tilesInRect;
  std::vector<std::tuple<int, int, bool, SDL_Rect>> tileRects =
	  this->getTileRects();
  Rect visibleMapRect = this->getVisibleMapExtents();

  std::optional<Rect> clippedRect = clipRect(rect, visibleMapRect);
  if (clippedRect != std::nullopt) {
	int tileSize = m_pMap->getTileSize();
	int numTilesWide = m_pMap->getWidth() / tileSize;
	int numTilesHigh = m_pMap->getHeight() / tileSize;

	for (unsigned int i = 0; i < tileRects.size(); i++) {
	  int x, y;
	  bool visible;
	  SDL_Rect rect;
	  std::tie(x, y, visible, rect) = tileRects[i];
	  if (visible && detectMBRCollision(rect, *clippedRect) != COLLISION_NONE) {
		tilesInRect.push_back(std::make_pair(x, numTilesHigh - y));
	  }
	}
  }

  return tilesInRect;
}

//! Gets the map held by the panel.
/**
 \return
   \li The map
*/
std::shared_ptr<Map2D> MapPanel::getMap() const { return m_pMap; }

//! Set the path on the panel.
/**
 \param pMap
   \li The map to set,
*/
void MapPanel::setMap(std::shared_ptr<Map2D> pMap) { m_pMap = pMap; }

//! \copydoc Wiget::getPosition
SDL_Rect MapPanel::getPosition() const { return {m_x, m_y, m_width, m_height}; }

} // namespace UI
} // namespace CapEngine

#include "tiledviewer.h"

#include <SDL_events.h>
#include <SDL_mouse.h>
#include <SDL_rect.h>
#include <capengine/VideoManager.h>
#include <capengine/camera2d.h>
#include <capengine/captypes.h>
#include <capengine/collision.h>
#include <capengine/locator.h>
#include <capengine/vector.h>

#include <cstdint>
#include <functional>
#include <optional>

namespace Game
{

namespace fs = std::filesystem;

TiledViewerState::TiledViewerState(uint32_t in_windowId, fs::path in_mapPath)
	: m_windowId(in_windowId),
	  m_texture(CapEngine::getNullTexturePtr()),
	  m_mapPath(std::move(in_mapPath)),
	  m_map(m_mapPath),
	  m_camera(0, 0),
	  m_mapRenderPosition(CapEngine::Rectangle{0, 0, 0, 0})
{
	auto& videoManager = CapEngine::Locator::getVideoManager();
	auto [width, height] = videoManager.getWindowResolution(m_windowId);
	m_camera.setWidth(width);
	m_camera.setHeight(height);

	// create the texture
	int textureWidth{m_map.width() * m_map.tileWidth()};
	int textureHeight{m_map.height() * m_map.tileHeight()};
	m_texture = CapEngine::Locator::getVideoManager().createTexturePtr(
		textureWidth, textureHeight);

	// set the logical dimensions of the window to match the dimensions of the
	// window
	// TODO there should be an upper abount on the size of the window maybe

	videoManager.setWindowLogicalResolution(
		m_windowId, m_map.width() * m_map.tileWidth(),
		m_map.height() * m_map.tileHeight());

	auto [logicalWidth, logicalHeight] =
		videoManager.getWindowLogicalResolution(m_windowId);
	m_camera.setWidth(logicalWidth);
	m_camera.setHeight(logicalHeight);

	CapEngine::Locator::getEventSubscriber().m_mouseButtonEventSignal.connect(
		[&](SDL_MouseButtonEvent in_event) {
			this->m_mouseState.handleMouseButtonEvent(*this, in_event);
		});

	CapEngine::Locator::getEventSubscriber().m_mouseWheelEventSignal.connect(
		[&](SDL_MouseWheelEvent in_event) {
			this->m_mouseState.handleMouseWheelEvent(*this, in_event);
		});
}

void TiledViewerState::render()
{
	auto& videoManager = CapEngine::Locator::getVideoManager();
	auto [windowWidth, windowHeight] =
		videoManager.getWindowResolution(m_windowId);

	auto [logicalWidth, logicalHeight] =
		videoManager.getWindowLogicalResolution(m_windowId);

	// render background first
	videoManager.drawFillRect(
		m_windowId, CapEngine::Rect{0, 0, logicalWidth, logicalHeight},
		CapEngine::Colour{0x20, 0x20, 0x20, 0xFF});

	// render the map
	m_map.render();

	CapEngine::Texture* mapTexture = m_map.texture();
	assert(mapTexture != nullptr);

	CapEngine::Rectangle mapRect{0, 0, logicalWidth, logicalHeight};

	mapRect = CapEngine::toScreenCoords(m_camera, mapRect, m_windowId, true);

	// last scroll position has been set so we need to keep make sure the
	// location of the map where the scroll wheel was used is still
	// the same location after zooming
	// if (m_mouseState.lastScrollPosition != std::nullopt) {
	//	std::cout << "Handling scroll in render()\n";
	//	std::cout << "Last render position: " << m_mapRenderPosition << "\n";
	//	std::cout << "Current render position: " << mapRect << "\n";
	//	// now we need to make sure x,y is is at last scroll position
	//	const int currentX =
	//		static_cast<int>(m_mouseState.lastScrollPosition->x) + mapRect.x;
	//	const int currentY =
	//		static_cast<int>(m_mouseState.lastScrollPosition->y) + mapRect.y;

	//	// but the render rectangles could be scaled differently so we need to
	//	// take that into account
	//	const auto widthRatio = mapRect.width / m_mapRenderPosition.width;
	//	const auto heightRatio = mapRect.height / m_mapRenderPosition.height;

	//	const int lastX =
	//		(static_cast<int>(m_mouseState.lastScrollPosition->x) +
	//		 m_mapRenderPosition.x) *
	//		widthRatio;
	//	const int lastY =
	//		(static_cast<int>(m_mouseState.lastScrollPosition->y) +
	//		 m_mapRenderPosition.y) *
	//		heightRatio;

	//	// get the displacement
	//	const auto displacement = CapEngine::Vector{currentX, currentY} -
	//							  CapEngine::Vector{lastX, lastY};

	//	m_camera.setPosition(static_cast<int>(displacement.x), static_cast<int>(displacement.y));

	//	// now translate the current render position
	//	const auto newPosition =
	//		CapEngine::Vector{static_cast<double>(mapRect.x),
	//						  static_cast<double>(mapRect.y)} +
	//		displacement;

	//	mapRect.x = static_cast<int>(newPosition.getX());
	//	mapRect.y = static_cast<int>(newPosition.getY());
	//	mapRect.width = mapRect.width;
	//	mapRect.height = mapRect.height;
	//	std::cout << "After scroll adjustment render position: " << mapRect
	//			  << "\n";

	//	m_mouseState.lastScrollPosition = std::nullopt;
	// }

	m_mapRenderPosition = mapRect;
	auto destRect = m_mapRenderPosition.toRect();

	videoManager.drawTexture(m_windowId, destRect, mapTexture);
}

void TiledViewerState::update(double /*ms*/)
{
	auto maybeMouseDisplacement = m_mouseState.displacement();
	if (maybeMouseDisplacement != std::nullopt) {
		auto [x, y] = m_camera.getPosition();
		auto newX = x + static_cast<int>((-1) * maybeMouseDisplacement->getX());
		auto newY = y + static_cast<int>(maybeMouseDisplacement->getY());
		m_camera.setPosition(newX, newY);
	}

	if (m_mouseState.scrollAmount != 0) {
		m_camera.zoom(static_cast<float>(m_mouseState.scrollAmount * 0.1));
	}

	m_mouseState.reset();
}

void TiledViewerState::handleMouseMotionEvent(SDL_MouseMotionEvent in_event) {}

void TiledViewerState::MouseState::handleMouseButtonEvent(
	TiledViewerState& in_testState, SDL_MouseButtonEvent in_event)
{
	auto window = CapEngine::Locator::getVideoManager().getWindow(
		in_testState.m_windowId);

	bool pressed =
		in_event.state == SDL_PRESSED && in_event.button == SDL_BUTTON_LEFT;

	// not pressed to pressed
	// store the mouse position just this once
	if (!this->pressed && pressed && SDL_GetMouseFocus() == window.m_window) {
		this->position = CapEngine::Vector{
			static_cast<double>(CapEngine::Locator::getMouse().getx()),
			static_cast<double>(
				CapEngine::Locator::getMouse().gety())};
		this->pressed = true;
	}

	// pressed to unpressed
	if (this->pressed && !pressed) {
		this->position = std::nullopt;
		this->pressed = false;
	}
}

void TiledViewerState::MouseState::handleMouseWheelEvent(
	TiledViewerState& in_testState, SDL_MouseWheelEvent in_event)
{
	auto window = CapEngine::Locator::getVideoManager().getWindow(
		in_testState.m_windowId);

	if (SDL_GetMouseFocus() == window.m_window) {
		// record how much was scrolled
		this->scrollAmount += in_event.y;

		// record where the scroll occurred in window coordinates (straight from
		// the sdl event)
		this->lastScrollPosition =
			CapEngine::Vector{static_cast<double>(in_event.x),
							  static_cast<double>(in_event.y)};
	}
}

std::optional<CapEngine::Vector> TiledViewerState::MouseState::displacement()
	const
{
	if (!this->pressed) {
		return std::nullopt;
	}

	auto currentPosition = CapEngine::Vector{
		static_cast<double>(CapEngine::Locator::getMouse().getx()),
		static_cast<double>(CapEngine::Locator::getMouse().gety())};

	return currentPosition - *this->position;
}

void TiledViewerState::MouseState::reset()
{
	this->position = CapEngine::Vector{
		static_cast<double>(CapEngine::Locator::getMouse().getx()),
		static_cast<double>(CapEngine::Locator::getMouse().gety())};

	this->scrollAmount = 0;
}

}  // namespace Game

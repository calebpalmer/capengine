#include "tiledviewer.h"

#include <SDL_events.h>
#include <SDL_mouse.h>
#include <capengine/VideoManager.h>
#include <capengine/camera2d.h>
#include <capengine/captypes.h>
#include <capengine/locator.h>

#include <cstdint>
#include <functional>

namespace Game
{

namespace fs = std::filesystem;

TiledViewerState::TiledViewerState(uint32_t in_windowId, fs::path in_mapPath)
	: m_windowId(in_windowId),
	  m_texture(CapEngine::getNullTexturePtr()),
	  m_mapPath(std::move(in_mapPath)),
	  m_map(m_mapPath),
	  m_camera(0, 0)
{
	auto& videoManager = CapEngine::Locator::getVideoManager();
	auto [width, height] = videoManager.getWindowResolution(m_windowId);
	m_camera.setWidth(width);
	m_camera.setHeight(height);
	m_position = CapEngine::Rectangle{0, 0, width, height};

	// create the texture
	int textureWidth{m_map.width() * m_map.tileWidth()};
	int textureHeight{m_map.height() * m_map.tileHeight()};
	m_texture = CapEngine::Locator::getVideoManager().createTexturePtr(
		textureWidth, textureHeight);

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
	videoManager.setWindowLogicalResolution(m_windowId, windowWidth,
											windowHeight);

	// render background first
	videoManager.drawFillRect(m_windowId,
							  CapEngine::Rect{0, 0, windowWidth, windowHeight},
							  CapEngine::Colour{0xFF, 0xFF, 0xFF, 0xFF});

	// render the map
	CapEngine::Rectangle windowDims{
		0, 0, windowWidth,
		windowHeight};  // TODO replace use of m_postion with this
	m_map.render();
	CapEngine::Texture* mapTexture = m_map.texture();

	SDL_Rect rect{0, 0, m_map.width() * m_map.tileWidth(),
				  m_map.height() * m_map.tileHeight()};

	assert(m_texture.get() != nullptr);
	videoManager.drawTexture(m_texture.get(), mapTexture, rect, rect);

	// render map to the window
	m_camera.setWidth(windowWidth);
	m_camera.setHeight(windowHeight);

	CapEngine::Rectangle mapDrawPosition{0, 0, windowWidth, windowHeight};
	m_position.width = windowWidth;
	m_position.height = windowHeight;

	auto viewingRect = m_camera.getViewingRectangle();
	mapDrawPosition =
		CapEngine::toScreenCoords(m_camera, mapDrawPosition, m_windowId, true);

	videoManager.drawTexture(m_windowId, mapDrawPosition.x, mapDrawPosition.y,
							 mapTexture);
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
		m_camera.zoom(static_cast<float>(m_mouseState.scrollAmount));
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
			static_cast<CapEngine::real>(CapEngine::Locator::getMouse().getx()),
			static_cast<CapEngine::real>(
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
		this->scrollAmount += in_event.y;
	}
}

std::optional<CapEngine::Vector> TiledViewerState::MouseState::displacement()
	const
{
	if (!this->pressed) {
		return std::nullopt;
	}

	auto currentPosition = CapEngine::Vector{
		static_cast<CapEngine::real>(CapEngine::Locator::getMouse().getx()),
		static_cast<CapEngine::real>(CapEngine::Locator::getMouse().gety())};

	return currentPosition - *this->position;
}

void TiledViewerState::MouseState::reset()
{
	this->position = CapEngine::Vector{
		static_cast<CapEngine::real>(CapEngine::Locator::getMouse().getx()),
		static_cast<CapEngine::real>(CapEngine::Locator::getMouse().gety())};

	this->scrollAmount = 0;
}

}  // namespace Game

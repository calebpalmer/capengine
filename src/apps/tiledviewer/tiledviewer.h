#ifndef GAME_TILEDVIEWER_H
#define GAME_TILEDVIEWER_H

#include <SDL_events.h>
#include <capengine/camera2d.h>
#include <capengine/captypes.h>
#include <capengine/collision.h>
#include <capengine/gamestate.h>
#include <capengine/tiledmap.h>
#include <capengine/vector.h>

#include <filesystem>
#include <string>

namespace Game
{
class TiledViewerState final : public CapEngine::GameState
{
   public:
	TiledViewerState(uint32_t in_windowId, std::filesystem::path in_mapPath);
	~TiledViewerState() override{};
	void render() override;
	void update(double ms) override;

	void handleMouseMotionEvent(SDL_MouseMotionEvent in_event);

	struct MouseState {
		void handleMouseButtonEvent(TiledViewerState& in_testState,
									SDL_MouseButtonEvent in_event);
		void handleMouseWheelEvent(TiledViewerState& in_testState,
								   SDL_MouseWheelEvent in_event);
		[[nodiscard]] std::optional<CapEngine::Vector> displacement() const;
		void reset();

		bool pressed = false;
		std::optional<CapEngine::Vector> position;
		std::optional<CapEngine::Vector> lastScrollPosition;
		int scrollAmount = 0;
	};
	friend class MouseState;

   private:
	uint32_t m_windowId;
	CapEngine::TexturePtr m_texture;
	std::filesystem::path m_mapPath;
	CapEngine::TiledMap m_map;
	CapEngine::Camera2d m_camera;
	MouseState m_mouseState;
	CapEngine::Rectangle m_mapRenderPosition;
};

}  // namespace Game

#endif /* GAME_TILEDVIEWER_H */

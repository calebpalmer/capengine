#ifndef CAPENGINE_TILESETPANEL_H
#define CAPENGINE_TILESETPANEL_H

#include "widget.h"
#include "tileset.h"

#include <memory>

namespace CapEngine { namespace UI {

		class TileSetPanel : public Widget {
		public:
			static std::shared_ptr<TileSetPanel> create(std::shared_ptr<TileSet> pTileSet);

			TileSetPanel(const TileSetPanel&) = delete;
			TileSetPanel& operator=(const TileSetPanel&) = delete;

			virtual void setPosition(int x, int y);
			virtual void setSize(int width, int height);
			virtual void render();

			virtual void handleMouseMotionEvent(SDL_MouseMotionEvent event) {}
			virtual void handleMouseButtonEvent(SDL_MouseButtonEvent event) {}
			virtual void handleMouseWheelEvent(SDL_MouseWheelEvent event) {}
			virtual void handleKeyboardEvent(SDL_KeyboardEvent event) {}
			virtual void handleWindowEvent(SDL_WindowEvent event) {}

		private:
			TileSetPanel(std::shared_ptr<TileSet> pTileSet);

			//! The owned TileSet
			std::shared_ptr<TileSet> m_pTileSet;
			//! The rendering rectangle
			SDL_Rect m_rect = {0, 0, 0, 0};
			//! The texture of the tiles from the tileset.
			TexturePtr m_pTileSetTexture;
		};
		
}}

#endif //CAPENGINE_TILESETPANEL_H

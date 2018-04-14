#ifndef CAPENGINE_TILESETPANEL_H
#define CAPENGINE_TILESETPANEL_H

#include "widget.h"
#include "tileset.h"

#include <memory>

namespace CapEngine { namespace UI {

		class TileSetPanel : public Widget {
		public:
			//! Used for caching draw location of a Tile
			struct TileLocation{
				//! The index into the TileSet off the tile to draw
				unsigned int index;
				//! The src rectangle from the tileset to draw
				SDL_Rect srcRect;
				//! The dst rectangle to draw to in the window
				SDL_Rect dstRect;
			};
			
		public:
			static std::shared_ptr<TileSetPanel> create(std::shared_ptr<TileSet> pTileSet);

			TileSetPanel(const TileSetPanel&) = delete;
			TileSetPanel& operator=(const TileSetPanel&) = delete;

			virtual void setPosition(int x, int y);
			virtual void setSize(int width, int height);
			virtual void render();

			virtual void handleMouseMotionEvent(SDL_MouseMotionEvent /*event*/) {}
			virtual void handleMouseButtonEvent(SDL_MouseButtonEvent event);
			virtual void handleMouseWheelEvent(SDL_MouseWheelEvent /*event*/) {}
			virtual void handleKeyboardEvent(SDL_KeyboardEvent /*event*/) {}
			virtual void handleWindowEvent(SDL_WindowEvent /*event*/) {}

		private:
			TileSetPanel(std::shared_ptr<TileSet> pTileSet);

			void updateDrawLocations();

			//! The owned TileSet
			std::shared_ptr<TileSet> m_pTileSet;
			//! The rendering rectangle
			SDL_Rect m_rect = {0, 0, 0, 0};
			//! The texture of the tiles from the tileset.
			TexturePtr m_pTileSetTexture;
			//! Stores cache of Tile draw locations
			std::vector<TileLocation> m_tiles;
			//! flag indicating if 
			bool m_updateDrawLocations = true;
		};
		
}}

#endif //CAPENGINE_TILESETPANEL_H

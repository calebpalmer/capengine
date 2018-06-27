#ifndef CAPENGINE_TILECOPYCONTROL_H
#define CAPENGINE_TILECOPYCONTROL_H

#include "control.h"
#include "tileset.h"
#include "captypes.h"

#include <boost/optional.hpp>

namespace CapEngine { namespace UI {

		class TileCopyControl : public Control {
		public:
		TileCopyControl(std::shared_ptr<TileSet> pTileSet, int index) :
			m_pTileSet(pTileSet), m_index(index) {}

			std::shared_ptr<const TileSet> getTileSet() const;
			int getIndex() const;

			virtual void render() override;

			virtual void handleMouseButtonEvent(SDL_MouseButtonEvent event) override;
			virtual void handleKeyboardEvent(SDL_KeyboardEvent event) override;
			virtual void setHandled(bool handled);

			bool isDragging() const;
			boost::optional<std::pair<int, int>> getInitialCoords() const;

		private:
			void loadTexture();
			void remove();
			
			//! The tileset
			std::shared_ptr<TileSet> m_pTileSet;
			//! The index in the tileset of the tile to set
			int m_index = -1;
			//! The texture to draw
			TexturePtr m_pTexture = getNullTexturePtr();
			//! flag indicating if this control has been handled.
			bool m_handled = false;
			//! if doing mouse drag
			bool m_isDragging = false;
			//! The initial coordinates of a drag if dragging.
			boost::optional<std::pair<int, int>> m_initialCoords;
		};

}}

#endif // CAPENGINE_TILECOPYCONTROL_H

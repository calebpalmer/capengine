#ifndef CAPENGINE_TILECOPYCONTROL_H
#define CAPENGINE_TILECOPYCONTROL_H

#include "control.h"
#include "tileset.h"
#include "captypes.h"

namespace CapEngine { namespace UI {

		class TileCopyControl : public Control {
		public:
		TileCopyControl(std::shared_ptr<TileSet> pTileSet, int index) :
			m_pTileSet(pTileSet), m_index(index) {}

			std::shared_ptr<const TileSet> getTileSet() const;
			int getIndex() const;

			virtual void render() override;

			virtual void handleMouseButtonEvent(SDL_MouseButtonEvent event) override;

		private:
			void loadTexture();
			
			//! The tileset
			std::shared_ptr<TileSet> m_pTileSet;
			//! The index in the tileset of the tile to set
			int m_index = -1;
			//! The texture to draw
			TexturePtr m_pTexture = getNullTexturePtr();
			//! flag indicating if this control has been handled.
			bool m_handled = false;
		};

}}

#endif // CAPENGINE_TILECOPYCONTROL_H

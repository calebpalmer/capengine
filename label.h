#ifndef LABEL_H
#define LABEL_H

#include "widget.h"
#include "captypes.h"
#include "fontmanager.h"

#include <string>
#include <memory>

namespace CapEngine { namespace UI {

		class Label : public Widget {
		public:
			Label(Widget* pParent, const std::string &text, const std::string &font="");

			 void setPosition(int x, int y) override;
			 void setSize(int width, int height) override;
			 void render() override;
		
		private:
			 int m_x = 0; //<! The x location of the label (upper left)
			 int m_y = 0; //<! The y location of the label (upper left)
			 int m_width = 0; //<! The width of the label
			 int m_height = 0; //<! The height of the label
			 std::string m_text = ""; //<! The text to display
			 std::string m_font = ""; //<! the font of the label

			 TexturePtr m_texture; //<! The surface
			 FontManager m_fontManager;  //<! fontmanager for getting text surfaces
		};
		
		
	} // namespace UI
} // namespace CapEngine

#endif // LABEL_H

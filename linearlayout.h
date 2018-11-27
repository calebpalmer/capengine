#ifndef CAPENGINE_LINEARLAYOUT_H
#define CAPENGINE_LINEARLAYOUT_H

#include "widget.h"

#include <vector>

namespace CapEngine { namespace UI {

		//! Layout the shows widgets linearly at even widths
		class LinearLayout : public Widget {
		public:
			//! The orientation of the layout
			enum class Orientation {
				Horizontal,
				Vertical
			};

			//! The Alignment of the layout
			enum class Alignment {
				Begin,
				Center,
				End
			};
			
		public:
			static std::shared_ptr<LinearLayout> create(Orientation orientation=Orientation::Horizontal);
			
			virtual ~LinearLayout() = default;

			virtual SDL_Rect getPosition() const override;
			virtual void setPosition(int x, int y) override;
			virtual void setSize(int width, int height) override;
			virtual void render() override;
			virtual void update(double ms) override;
			virtual void setWindowId(Uint32 windowId) override;
			virtual std::vector<std::shared_ptr<Widget>> getChildren() override;

			virtual void handleMouseMotionEvent(SDL_MouseMotionEvent event) override;
			virtual void handleMouseButtonEvent(SDL_MouseButtonEvent event) override;
			virtual void handleMouseWheelEvent(SDL_MouseWheelEvent event) override;
			virtual void handleKeyboardEvent(SDL_KeyboardEvent event) override;
			virtual void handleWindowEvent(SDL_WindowEvent event) override;
			virtual void handleTextInputEvent(SDL_TextInputEvent event) override;

			void addWidget(std::shared_ptr<Widget> pWidget);

			Alignment getAlignment() const;
			void setAlignment(Alignment alignment);

		private:
			LinearLayout(Orientation orientation);

			void updateChildren();

			//! The orientation of the layout
			Orientation m_orientation = Orientation::Horizontal;
			//! The alignment of the layout.
			Alignment m_alignment = Alignment::Center;
			//! holds the widgets in this layout
			std::vector<std::shared_ptr<Widget>> m_widgets;
			//! position and size
			SDL_Rect m_rect;
			//! flag indicating children need size and position updates
			bool m_updateChildren = false;
		};
}}

#endif // CAPENGINE_LINEARLAYOUT_H

#ifndef CAPENGINE_HORIZONTALLAYOUT_H
#define CAPENGINE_HORIZONTALLAYOUT_H

#include "widget.h"

#include <vector>

namespace CapEngine { namespace UI {

		//! Layout the shows widgets horizontally at even widths
		class HorizontalLayout : public Widget {
		public:
			static std::shared_ptr<HorizontalLayout> create();
			
			virtual ~HorizontalLayout() = default;

			virtual void setPosition(int x, int y) override;
			virtual void setSize(int width, int height) override;
			virtual void render() override;
			virtual void update(double ms) override;
			virtual void setWindowId(Uint32 windowId) override;

			virtual void handleMouseMotionEvent(SDL_MouseMotionEvent event) override;
			virtual void handleMouseButtonEvent(SDL_MouseButtonEvent event) override;
			virtual void handleMouseWheelEvent(SDL_MouseWheelEvent event) override;
			virtual void handleKeyboardEvent(SDL_KeyboardEvent event) override;
			virtual void handleWindowEvent(SDL_WindowEvent event) override;

			void addWidget(std::shared_ptr<Widget> pWidget);

		private:
			HorizontalLayout() = default;

			void updateChildren();

			//! holds the widgets in this layout
			std::vector<std::shared_ptr<Widget>> m_widgets;
			//! position and size
			SDL_Rect m_rect;
			//! flag indicating children need size and position updates
			bool m_updateChildren = false;

		};
}}

#endif // CAPENGINE_HORIZONTALLAYOUT_H

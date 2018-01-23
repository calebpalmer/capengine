#ifndef ABSOLUTELAYOUT_H
#define ABSOLUTELAYOUT_H

#include "widget.h"

#include <memory>
#include <vector>

namespace CapEngine { namespace UI {

	class WindowWidget;

	//! Class with absolute control of contained widget locations
	class AbsoluteLayout : public Widget {
	public:
		AbsoluteLayout(Widget* pParent);

		// Widget overrides
		void render() override;
		void setPosition(int x, int y) override;
		void setSize(int x, int y) override;
    void handleMouseMotionEvent(SDL_MouseMotionEvent event);
    void handleMouseButtonEvent(SDL_MouseButtonEvent event);
    void handleMouseWheelEvent(SDL_MouseWheelEvent event);
    void handleKeyboardEvent(SDL_KeyboardEvent event);
    void handleWindowEvent(SDL_WindowEvent event);
		

		void addWidget(std::shared_ptr<Widget>, int x, int y, int width, int height);
		void removeWidget(std::shared_ptr<Widget>);
		

	private:
		//! struct holding widget location info
		struct WidgetLocation{
			std::shared_ptr<Widget> pWidget;
			int x; //<! x location of widget within layout
			int y; //<! y location of widget within layout
			int width; //<! The width of the widget
			int height; //<! The height of the widget
		};

		//! The widgets in this layout
		std::vector<WidgetLocation> m_widgets;
		int m_x; //<! The x location within the window
		int m_y; //<! The y location withing the window
		int m_width; //<! The width
		int m_height; //<! The height
		
	};

	} // namespace UI
} // namespace CapEngine

#endif // ABSOLUTELAYOUT_H

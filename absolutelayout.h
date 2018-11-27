#ifndef ABSOLUTELAYOUT_H
#define ABSOLUTELAYOUT_H

#include "widget.h"
#include "uicommon.h"

#include <memory>
#include <vector>

namespace CapEngine { namespace UI {

	class WindowWidget;

	//! Class with absolute control of contained widget locations
	class AbsoluteLayout : public Widget {
	public:
		static std::shared_ptr<AbsoluteLayout> create();

		// Widget overrides
		SDL_Rect getPosition() const override;
		void render() override;
		void setPosition(int x, int y) override;
		void setSize(int x, int y) override;
		void setWindowId(Uint32 windowId) override;
		std::vector<std::shared_ptr<Widget>> getChildren() override;
		
    void handleMouseMotionEvent(SDL_MouseMotionEvent event) override;
    void handleMouseButtonEvent(SDL_MouseButtonEvent event) override;
    void handleMouseWheelEvent(SDL_MouseWheelEvent event) override;
    void handleKeyboardEvent(SDL_KeyboardEvent event) override;
    void handleWindowEvent(SDL_WindowEvent event) override;
		void handleTextInputEvent(SDL_TextInputEvent event) override;
		
		void addWidget(std::shared_ptr<Widget>, int x, int y, int width, int height,
									 Unit widthUnit = Unit::Pixels, Unit heightUnit = Unit::Pixels) ;
		void removeWidget(std::shared_ptr<Widget>);

	private:
		//! struct holding widget location info
		struct WidgetLocation{
			std::shared_ptr<Widget> pWidget;
			int x; //<! x location of widget within layout
			int y; //<! y location of widget within layout
			int width; //<! The width of the widget
			int height; //<! The height of the widget
			Unit widthUnit; //<! The unit of the width
			Unit heightUnit; //<! The unit of the height
		};

	private:
		AbsoluteLayout();

		void updateWidgetPositions();

		bool m_updateWidgets = false;
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

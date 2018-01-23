#ifndef WINDOWWIDGET_H
#define WINDOWWIDGET_H

#include "widget.h"
#include "colour.h"

#include <string>
#include <memory>
#include <limits>
#include <boost/signals2.hpp>

namespace CapEngine {

	class EventSubscriber;
	
	namespace UI {

		class WindowWidget : public Widget {

		public:
			WindowWidget(const std::string &name, int width, int height, bool resizable=true);
			~WindowWidget();

			// widget overrides
			virtual void setPosition(int x, int y) override;
			virtual void setSize(int width, int height) override;
			virtual void render() override;

			void registerSignals(EventSubscriber &eventSubscriber);
			virtual void handleMouseMotionEvent(SDL_MouseMotionEvent event) override;
			virtual void handleMouseButtonEvent(SDL_MouseButtonEvent event) override;
			virtual void handleMouseWheelEvent(SDL_MouseWheelEvent event) override;
			virtual void handleKeyboardEvent(SDL_KeyboardEvent event) override;
			virtual void handleWindowEvent(SDL_WindowEvent event) override;

			void show();
			void close();
			void setLayout(std::shared_ptr<Widget> pLayout);

		private:
			std::string m_windowName; //<! The name of the window
			int m_x = 0; //<! The x position of the window
			int m_y = 0; //<! The y position of the window
			int m_width = 0; //<! The width of the window
			int m_height = 0; //<! The height of the window
			bool m_resizable = true; //<! flag indicating if the window is resizable
			bool m_shown = false; //<! flag indicacting if window has been shown yet
			std::shared_ptr<Widget> m_pLayout; //<! The layout of the window
			Colour m_backgroundColour = { 150, 150, 150, 255 };

			// connections
			//! connection for mouse motion event signals
			boost::signals2::scoped_connection m_handleMouseMotionConnection;
			boost::signals2::scoped_connection m_handleMouseButtonConnection;
			boost::signals2::scoped_connection m_handleMouseWheelConnection;
			boost::signals2::scoped_connection m_handleKeyboardConnection;
			boost::signals2::scoped_connection m_handleWindowConnection;
		};

	}
}

#endif // WINDOWWIDGET

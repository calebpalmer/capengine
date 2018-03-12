#ifndef GRIDLAYOUT_H
#define GRIDLAYOUT_H

#include "widget.h"
#include "captypes.h"

#include <memory>
#include <functional>
#include <boost/optional.hpp>

namespace CapEngine { namespace UI {

		// Layout of widgets in a Grid
		class GridLayout : public Widget {

		public:
			static std::shared_ptr<GridLayout> create(int numRows, int numCols);
			
			virtual void setPosition(int x, int y) override;
			virtual void setSize(int width, int height) override;
			virtual void render() override;
			virtual void update(double ms) override;

			virtual void handleMouseMotionEvent(SDL_MouseMotionEvent event) override;
			virtual void handleMouseButtonEvent(SDL_MouseButtonEvent event) override;
			virtual void handleMouseWheelEvent(SDL_MouseWheelEvent event) override;
			virtual void handleKeyboardEvent(SDL_KeyboardEvent event) override;
			virtual void handleWindowEvent(SDL_WindowEvent event) override;

			virtual void setWindowId(Uint32 windowId) override;

			void addWidget(std::shared_ptr<Widget>, int row, int column, bool replaceExisting=false);
			int getNumRows() const;
			int getNumColumns() const;
			std::shared_ptr<Widget> getWidget(int row, int column);

		private:
			GridLayout(int numRows, int numColumns);

			void updateChildren();

			// members
			//! The widgets this layout holds
			std::vector<std::vector<std::shared_ptr<Widget>>> m_widgetGrid;
			//! The number of rows in the layout
			int m_numRows;
			//! The number of columns in the layout
			int m_numColumns;
			// The position of the layout
			Rect m_position = { 0, 0, 0, 0 };
			
		};
}}

#endif // GRIDLAYOYT_H

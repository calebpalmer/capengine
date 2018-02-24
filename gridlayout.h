#ifndef GRIDLAYOUT_H
#define GRIDLAYOUT_H

#include "widget.h"
#include "captypes.h"

#include <memory>

namespace CapEngine { namespace UI {

		// Layout of widgets in a Grid
		class GridLayout : public Widget {
		public:
			static std::shared_ptr<GridLayout> create(int numRows, int numCols);
			
			virtual void setPosition(int x, int y) ;
			virtual void setSize(int width, int height);
			virtual void render();

			virtual void handleMouseMotionEvent(SDL_MouseMotionEvent event) {}
			virtual void handleMouseButtonEvent(SDL_MouseButtonEvent event) {}
			virtual void handleMouseWheelEvent(SDL_MouseWheelEvent event) {}
			virtual void handleKeyboardEvent(SDL_KeyboardEvent event) {}
			virtual void handleWindowEvent(SDL_WindowEvent event) {}

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

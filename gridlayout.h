#ifndef GRIDLAYOUT_H
#define GRIDLAYOUT_H

#include "widget.h"
#include "captypes.h"

#include <memory>
#include <functional>
#include <optional>
#include <boost/optional.hpp>

namespace CapEngine { namespace UI {

		// Layout of widgets in a Grid
		class GridLayout : public Widget {

		public:
			static std::shared_ptr<GridLayout> create(int numRows,
				int numCols,
				boost::optional<std::vector<int>> maybeRowHeights = boost::none,
			  boost::optional<std::vector<int>> maybeColWidths = boost::none);

			SDL_Rect getPosition() const override;
			void setPosition(int x, int y) override;
			void setSize(int width, int height) override;
			void render() override;
			void update(double ms) override;
			std::vector<std::shared_ptr<Widget>> getChildren() override;
			void setBorder(BorderStyle borderStyle, unsigned int borderWidth) override;

			void handleMouseMotionEvent(SDL_MouseMotionEvent event) override;
			void handleMouseButtonEvent(SDL_MouseButtonEvent event) override;
			void handleMouseWheelEvent(SDL_MouseWheelEvent event) override;
			void handleKeyboardEvent(SDL_KeyboardEvent event) override;
			void handleWindowEvent(SDL_WindowEvent event) override;
			void handleTextInputEvent(SDL_TextInputEvent event) override;			

			void setWindowId(Uint32 windowId) override;

			void addWidget(std::shared_ptr<Widget>, int row, int column, bool replaceExisting=false);
			void removeWidget(int row, int column);
			void replaceWidget(std::shared_ptr<Widget> pWidget, int row, int column);
			int getNumRows() const;
			int getNumColumns() const;
			std::shared_ptr<Widget> getWidget(int row, int column);

		private:
			GridLayout(int numRows, int numColumns,
			boost::optional<std::vector<int>> maybeRowHeights = boost::none,
		    boost::optional<std::vector<int>> maybeColWidths = boost::none);

			void updateChildren();
			void updateWidgetPosition(Widget &widget, const SDL_Rect &rect);
			void renderWidget(std::shared_ptr<Widget>& widget, const SDL_Rect &rect);
			void updateCellBoxes();

			using widgetinfo_t = std::pair<SDL_Rect, std::shared_ptr<Widget>>;
			using widgetgrid_t = std::vector<std::vector<widgetinfo_t>>;

			// members
			//! The widgets this layout holds
			widgetgrid_t m_widgetGrid;
			//! The row heights as percentages
			boost::optional<std::vector<int>> m_rowHeights;
			//! the column widths as percentages
			boost::optional<std::vector<int>> m_colWidths;
			//! The number of rows in the layout
			int m_numRows = 0;
			//! The number of columns in the layout
			int m_numColumns = 0;
			//! The position of the layout
			Rect m_position = { 0, 0, 0, 0 };
			//! The position of the layout including the border
			unsigned int m_borderWidth = 0;
			//! The border style
			BorderStyle m_borderStyle = BorderStyle::None;
		};
}}

#endif // GRIDLAYOYT_H

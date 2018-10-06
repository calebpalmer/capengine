#include "gridlayout.h"
#include "locator.h"
#include "logger.h"

#include "CapEngineException.h"

#include <algorithm>

namespace CapEngine { namespace UI {

//! Constructor
/** 
		\param numRows
		\li The number of rows 
		\param numColumns
		\li The number of columns
		\param maybeRowHeights
		\li optional row height configuration
		\param maybeColWidths
		\li optional column width configuration
*/
GridLayout::GridLayout(int numRows, 
											 int numColumns,
											 boost::optional<std::vector<int>> maybeRowHeights,
											 boost::optional<std::vector<int>> maybeColWidths) 
	: m_numRows(numRows), m_numColumns(numColumns)
{
	for(int i = 0; i < numRows; i++){
		m_widgetGrid.push_back(std::vector<widgetinfo_t>(numColumns));
	}

	// row height configuration
	if(maybeRowHeights != boost::none){
		m_rowHeights = std::move(*maybeRowHeights);

		if(m_rowHeights->size() != static_cast<unsigned int>(numRows)){
			CAP_LOG(Locator::logger, "Row height configuration does not match number of rows.", Logger::CWARNING);
			m_rowHeights = boost::none;
		}
		
		int totalPercentage = 0;
		for(auto && i : *m_rowHeights){
			totalPercentage += i;
		}
		
		if(totalPercentage != 100){
			CAP_LOG(Locator::logger, "Row height configuration does not add up to 100%", Logger::CWARNING);
			m_rowHeights = boost::none;
		}
	}

	// column width configuration
	if(maybeColWidths != boost::none){
		m_colWidths = std::move(*maybeColWidths);
		
		if(m_colWidths->size() != static_cast<unsigned int>(numColumns)){
			CAP_LOG(Locator::logger, "Column width configuration does not match number of columns", Logger::CWARNING);
			m_colWidths = boost::none;
		}

		int totalPercentage = 0;
		for(auto && i : *m_colWidths){
			totalPercentage += i;
		}
		
		if(totalPercentage != 100){
			CAP_LOG(Locator::logger, "Column width configuration does not add up to 100%", Logger::CWARNING);
			m_rowHeights = boost::none;
		}
		
	}
}

//! creates a GridLayout
/** 
		\param numRows
		\liThe number of rows 
		\param numColumns
		\li The number of columns
		\param maybeRowHeights
		\li optional row height configuration
		\param maybeColWidths
		\li optional column width configuration
		\return 
		\li The grid layout
*/
std::shared_ptr<GridLayout> GridLayout::create(int numRows, int numColumns,
																							 boost::optional<std::vector<int>> maybeRowHeights,
																							 boost::optional<std::vector<int>> maybeColWidths){
	return std::shared_ptr<GridLayout>(new GridLayout(numRows, numColumns, maybeRowHeights, maybeColWidths));
}


//! @copydoc Widget::setPosition()
void GridLayout::setPosition(int x, int y){
	m_position.x = x;
	m_position.y = y;

	updateChildren();
}

//! @copydoc Widget::setSize()
void GridLayout::setSize(int width, int height){
	m_position.w = width;
	m_position.h = height;;

	updateChildren();
}

//! \copydoc Widget::setWindowId
void GridLayout::setWindowId(Uint32 windowId){
	m_windowId = windowId;

	for(int i = 0; i < m_numRows; i++){
		for(int j = 0; j < m_numColumns; j++){
			if(m_widgetGrid[i][j].second != nullptr){
				m_widgetGrid[i][j].second->setWindowId(windowId);
			}
		}
	}
}

//! \copydoc GridLayout::update
void GridLayout::update(double ms){
	for (auto && row : m_widgetGrid){
		for (auto && cell : row){
			if(cell.second != nullptr){
				cell.second->update(ms);
			}
		}
	}
}

//! @copydoc Widget::render()
void GridLayout::render(){
	for (auto && row : m_widgetGrid){
		for (auto && cell : row){
				this->renderWidget(cell.second, cell.first);
		}
	}
}


//! return the number of rows in the grid
/** 
		\return The number of rows in the grid
*/
int GridLayout::getNumRows() const {
	return m_numRows;
}

//! return the number of columns in the grid
/** 
		\return The number of columns in the grid
*/
int GridLayout::getNumColumns() const {
	return m_numColumns;
}

//! \copydoc Widget::handleMouseMotionEvent
void GridLayout::handleMouseMotionEvent(SDL_MouseMotionEvent event){
	for(int i = 0; i < m_numRows; i++){
		for(int j = 0; j < m_numColumns; j++){
			if(m_widgetGrid[i][j].second != nullptr){
				m_widgetGrid[i][j].second->handleMouseMotionEvent(event);
			}
		}
	}
}

//! \copydoc Widget::handleMouseButtonEvent
void GridLayout::handleMouseButtonEvent(SDL_MouseButtonEvent event){
	for(int i = 0; i < m_numRows; i++){
		for(int j = 0; j < m_numColumns; j++){
			if(m_widgetGrid[i][j].second != nullptr){
				m_widgetGrid[i][j].second->handleMouseButtonEvent(event);
			}
		}
	}
}

//! \copydoc Widget::handleMouseWheelEvent
void GridLayout::handleMouseWheelEvent(SDL_MouseWheelEvent event){
	for(int i = 0; i < m_numRows; i++){
		for(int j = 0; j < m_numColumns; j++){
			if(m_widgetGrid[i][j].second != nullptr){
				m_widgetGrid[i][j].second->handleMouseWheelEvent(event);
			}
		}
	}
}

//! \copydoc Widget::handleKeyboardEvent
void GridLayout::handleKeyboardEvent(SDL_KeyboardEvent event){
	for(int i = 0; i < m_numRows; i++){
		for(int j = 0; j < m_numColumns; j++){
			if(m_widgetGrid[i][j].second != nullptr){
				m_widgetGrid[i][j].second->handleKeyboardEvent(event);
			}
		}
	}
}

//! \copydoc Widget::handleWindowEvent
void GridLayout::handleWindowEvent(SDL_WindowEvent event){
	for(int i = 0; i < m_numRows; i++){
		for(int j = 0; j < m_numColumns; j++){
			if(m_widgetGrid[i][j].second != nullptr){
				m_widgetGrid[i][j].second->handleWindowEvent(event);
			}
		}
	}
}

//! \copydoc Widget::handleTextInputEvent
void GridLayout::handleTextInputEvent(SDL_TextInputEvent event){
	for(int i = 0; i < m_numRows; i++){
		for(int j = 0; j < m_numColumns; j++){
			if(m_widgetGrid[i][j].second != nullptr){
				m_widgetGrid[i][j].second->handleTextInputEvent(event);
			}
		}
	}
}

//! Return the wiget in the given grid location
/** 
		\param row - The row of the widget
		\param column - The column of the Widget
		\return - The widget
*/
std::shared_ptr<Widget> GridLayout::getWidget(int row, int column){
	if(row >= m_numRows || column >= m_numColumns)
		CAP_THROW(CapEngineException("Index out of bound"));

	return m_widgetGrid[row][column].second;
}


//! Add a widget to thelayout
/** 
		\param pWidget - The widget to add
		\param row - The row to add the widget to
		\param column - The column to add the widget to
		\param replaceExisting
		\li - If true, replace the existing widget if there is one 
		\li - If false, throw an exception if one already exists at given location
		\return 
*/
void GridLayout::addWidget(std::shared_ptr<Widget> pWidget, int row, int column,
													 bool replaceExisting)
{
	if(row >= m_numRows || column >= m_numColumns)
		CAP_THROW(CapEngineException("Index out of bound"));

	if(replaceExisting){
		m_widgetGrid[row][column] = std::make_pair(SDL_Rect{}, pWidget);
		pWidget->setParent(this);
		pWidget->setWindowId(this->m_windowId);
	}

	else{
		if(m_widgetGrid[row][column].second == nullptr){
			m_widgetGrid[row][column] = std::make_pair(SDL_Rect{}, pWidget);
			pWidget->setParent(this);
			pWidget->setWindowId(this->m_windowId);
		}
	}

	this->updateChildren();
}

//! Updates the size and position of children widgets
void GridLayout::updateChildren(){
	this->updateCellBoxes();
}

//! \copydoc Widget::getChildren
std::vector<std::shared_ptr<Widget>> GridLayout::getChildren(){
	std::vector<std::shared_ptr<Widget>> widgets;

	for(auto && row : m_widgetGrid){
		for(auto && cell : row){
			if(cell.second != nullptr){
				widgets.push_back(cell.second);
			}
		}
	}

	return widgets;
}

//! \copydoc Widget::getPosition
SDL_Rect GridLayout::getPosition() const {
	return m_position;
}

//! \copydoc Widget::setBorder
void GridLayout::setBorder(BorderStyle borderStyle, unsigned int borderWidth){
	m_borderStyle = borderStyle;
	m_borderWidth = borderWidth;
}

//! updates the rect of the cells
void GridLayout::updateCellBoxes() {
	int cellWidth = m_position.w / m_numColumns;
	int cellHeight = m_position.h / m_numRows;	

	int totalWidth = cellWidth * m_numColumns;
	int totalHeight = cellHeight * m_numRows;

	int y = 0;
	for(unsigned int i = 0; i < static_cast<unsigned int>(m_numRows); i++){
		// calculate current cell height
		int currentCellHeight = cellHeight;
		if(m_rowHeights != boost::none){
			assert(i < m_rowHeights->size());
			currentCellHeight = static_cast<double>(totalHeight) * (static_cast<double>((*m_rowHeights)[i]) / 100.0);
		}

		int x = 0;
		for(unsigned int j = 0; j < static_cast<unsigned int>(m_numColumns); j++){
			// calculate current cell width
			int currentCellWidth = cellWidth;
			if(m_colWidths != boost::none){
				assert(j < m_colWidths->size());
				currentCellWidth = static_cast<double>(totalWidth) * (static_cast<double>((*m_colWidths)[j]) / 100.0);
			}

			assert(i < m_widgetGrid.size());
			assert(j < m_widgetGrid[i].size());
			
			SDL_Rect position{x, y, currentCellWidth, currentCellHeight};
			m_widgetGrid[i][j].first = position;

			if(m_widgetGrid[i][j].second != nullptr){
				this->updateWidgetPosition(*m_widgetGrid[i][j].second, position);
			}

			// increment x position
			x += currentCellWidth;
		}

		// increment y position
		y += currentCellHeight;
	}
}

//! Updates the position of a child widget.
/** 
 \param widget
   \li The widget to update.
 \param rect
   \li The position of the widget (including any borders)
*/
void GridLayout::updateWidgetPosition(Widget &widget, const SDL_Rect &rect){
	if(m_borderStyle == BorderStyle::Solid && m_borderWidth > 0){
		widget.setPosition(rect.x + m_borderWidth, rect.y + m_borderWidth);
		widget.setSize(rect.w - (2 * m_borderWidth), rect.h - (2 * m_borderWidth));
	}
	else{
		widget.setPosition(rect.x, rect.y);
		widget.setSize(rect.w, rect.h);
	}
}

//! Render a widget and border if there is one.
/** 
 \param widget
   \li The widget to render.
 \param rect
   \li The position of the widget including border.
*/
void GridLayout::renderWidget(std::shared_ptr<Widget>& widget, const SDL_Rect &rect){

	Locator::videoManager->drawLine(m_windowId, rect.x,
																	rect.y,
																	rect.x + rect.w,
																	rect.y,
																	Colour{0, 0, 0, 255});

	Locator::videoManager->drawLine(m_windowId, rect.x,
																	rect.y,
																	rect.x,
																	rect.y  + rect.h,
																	Colour{0, 0, 0, 255});

	Locator::videoManager->drawLine(m_windowId, rect.x,
																	rect.y + rect.h,
																	rect.x + rect.w,
																	rect.y  + rect.h,
																	Colour{0, 0, 0, 255});

	Locator::videoManager->drawLine(m_windowId, rect.x + rect.w,
																	rect.y,
																	rect.x + rect.w,
																	rect.y  + rect.h,
																	Colour{0, 0, 0, 255});

	if(widget != nullptr){
		widget->render();
	}
}
	
}} // CapEngine::UI


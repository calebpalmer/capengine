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
		m_widgetGrid.push_back(std::vector<std::shared_ptr<Widget>>(numColumns));
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
	m_position.h = height;

	updateChildren();
}

//! \copydoc Widget::setWindowId
void GridLayout::setWindowId(Uint32 windowId){
	m_windowId = windowId;

	for(int i = 0; i < m_numRows; i++){
		for(int j = 0; j < m_numColumns; j++){
			if(m_widgetGrid[i][j].get() != nullptr){
				m_widgetGrid[i][j]->setWindowId(windowId);
			}
		}
	}
}

//! \copydoc GridLayout::update
void GridLayout::update(double ms){
	for (auto && row : m_widgetGrid){
		for (auto && col : row){
			if(col != nullptr)
				col->update(ms);
		}
	}
}

//! @copydoc Widget::render()
void GridLayout::render(){
	for (auto && row : m_widgetGrid){
		for (auto && col : row){
			if(col != nullptr)
				col->render();
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
			if(m_widgetGrid[i][j].get() != nullptr){
				m_widgetGrid[i][j]->handleMouseMotionEvent(event);
			}
		}
	}
}

//! \copydoc Widget::handleMouseButtonEvent
void GridLayout::handleMouseButtonEvent(SDL_MouseButtonEvent event){
	for(int i = 0; i < m_numRows; i++){
		for(int j = 0; j < m_numColumns; j++){
			if(m_widgetGrid[i][j].get() != nullptr){
				m_widgetGrid[i][j]->handleMouseButtonEvent(event);
			}
		}
	}
}

//! \copydoc Widget::handleMouseWheelEvent
void GridLayout::handleMouseWheelEvent(SDL_MouseWheelEvent event){
	for(int i = 0; i < m_numRows; i++){
		for(int j = 0; j < m_numColumns; j++){
			if(m_widgetGrid[i][j].get() != nullptr){
				m_widgetGrid[i][j]->handleMouseWheelEvent(event);
			}
		}
	}
}

//! \copydoc Widget::handleKeyboardEvent
void GridLayout::handleKeyboardEvent(SDL_KeyboardEvent event){
	for(int i = 0; i < m_numRows; i++){
		for(int j = 0; j < m_numColumns; j++){
			if(m_widgetGrid[i][j].get() != nullptr){
				m_widgetGrid[i][j]->handleKeyboardEvent(event);
			}
		}
	}
}

//! \copydoc Widget::handleWindowEvent
void GridLayout::handleWindowEvent(SDL_WindowEvent event){
	for(int i = 0; i < m_numRows; i++){
		for(int j = 0; j < m_numColumns; j++){
			if(m_widgetGrid[i][j].get() != nullptr){
				m_widgetGrid[i][j]->handleWindowEvent(event);
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

	return m_widgetGrid[row][column];
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
		m_widgetGrid[row][column] = pWidget;
		pWidget->setParent(this);
		pWidget->setWindowId(this->m_windowId);
	}

	else{
		if(m_widgetGrid[row][column] == nullptr){
			m_widgetGrid[row][column] = pWidget;
			pWidget->setParent(this);
			pWidget->setWindowId(this->m_windowId);
		}
	}

	this->updateChildren();
}



//! Updates the size and position of children widgets
void GridLayout::updateChildren(){
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

			if(m_widgetGrid[i][j].get() != nullptr){
				m_widgetGrid[i][j]->setPosition(x, y);
				m_widgetGrid[i][j]->setSize(currentCellWidth, currentCellHeight);
			}

			// increment x position
			x += currentCellWidth;
		}

		// increment y position
		y += currentCellHeight;
	}
}


}} // CapEngine::UI


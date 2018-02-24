#include "gridlayout.h"

#include "CapEngineException.h"

namespace CapEngine { namespace UI {


//! Constructor
/** 
 \param numRows - The number of rows 
 \param numColumns - The number of columns
*/
GridLayout::GridLayout(int numRows, int numColumns) :
	m_numRows(numRows), m_numColumns(numColumns)
{
	for(int i = 0; i < numRows; i++){
		m_widgetGrid.push_back(std::vector<std::shared_ptr<Widget>>(numColumns));
	}
}

//! creates a GridLayout
/** 
 \param numRows - The number of rows 
 \param numColumns - The number of columns
 \return - The grid layout
*/
std::shared_ptr<GridLayout> GridLayout::create(int numRows, int numColumns){
	return std::shared_ptr<GridLayout>(new GridLayout(numRows, numColumns));
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

	for(int i = 0; i < m_numRows; i++){
		for(int j = 0; j < m_numColumns; j++){

			int x = j * cellWidth;
			int y = i * cellHeight;

			if(m_widgetGrid[i][j].get() != nullptr){
				m_widgetGrid[i][j]->setPosition(x, y);
				m_widgetGrid[i][j]->setSize(cellWidth, cellHeight);
			}
		}
	}
}


}} // CapEngine::UI


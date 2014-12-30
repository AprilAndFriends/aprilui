/// @file
/// @version 4.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/aprilUtil.h>
#include <gtypes/Rectangle.h>
#include <hltypes/hlog.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "Dataset.h"
#include "ObjectGridView.h"
#include "ObjectGridViewCell.h"
#include "ObjectGridViewRow.h"
#include "ObjectGridViewRowTemplate.h"
#include "ObjectScrollArea.h"
#include "ObjectScrollBarV.h"

namespace aprilui
{
	harray<PropertyDescription> GridView::_propertyDescriptions;

	GridView::GridView(chstr name) : SelectionContainer(name)
	{
		this->spacingWidth = 4.0f;
		this->spacingHeight = 4.0f;
		this->rowTemplate = NULL;
	}

	GridView::GridView(const GridView& other) : SelectionContainer(other)
	{
		this->spacingWidth = other.spacingWidth;
		this->spacingHeight = other.spacingHeight;
		// TODO - might want to clone this
		this->rowTemplate = NULL;
	}

	GridView::~GridView()
	{
	}

	Object* GridView::createInstance(chstr name)
	{
		return new GridView(name);
	}

	harray<PropertyDescription> GridView::getPropertyDescriptions()
	{
		if (GridView::_propertyDescriptions.size() == 0)
		{
			GridView::_propertyDescriptions += PropertyDescription("spacing_width", PropertyDescription::FLOAT);
			GridView::_propertyDescriptions += PropertyDescription("spacing_height", PropertyDescription::FLOAT);
		}
		return (SelectionContainer::getPropertyDescriptions() + GridView::_propertyDescriptions);
	}

	void GridView::setSpacingWidth(float value)
	{
		if (this->spacingWidth != value)
		{
			this->spacingWidth = value;
			this->_updateDisplay();
		}
	}

	void GridView::setSpacingHeight(float value)
	{
		if (this->spacingHeight != value)
		{
			this->spacingHeight = value;
			this->_updateDisplay();
		}
	}

	GridViewCell* GridView::getSelected()
	{
		return (hbetweenIE(this->selectedIndex, 0, this->cells.size()) ? this->cells[this->selectedIndex] : NULL);
	}

	int GridView::getRowCount()
	{
		return this->rows.size();
	}

	int GridView::getCellCount()
	{
		return this->cells.size();
	}

	int GridView::getItemCount()
	{
		return this->cells.size();
	}

	void GridView::_updateDisplay()
	{
		for_iter (i, 0, this->rows.size())
		{
			this->_updateRow(i);
		}
		this->_updateScrollArea();
	}

	void GridView::_updateRow(int index)
	{
		if (hbetweenIE(index, 0, this->rows.size()))
		{
			this->rows[index]->setY(index * (this->itemHeight + this->spacingHeight));
			this->rows[index]->setHeight(this->itemHeight);
			int offset = index * this->rowTemplate->_gridViewCells.size();
			for_iter (i, 0, this->rows[index]->_gridViewCells.size())
			{
				this->_updateItem(i + offset);
			}
		}
	}

	void GridView::_updateItem(int index)
	{
		if (hbetweenIE(index, 0, this->cells.size()))
		{
			if (this->selectedIndex != index)
			{
				this->cells[index]->_hoverColor = this->hoverColor;
				this->cells[index]->_pushedColor = this->pushedColor;
			}
			else
			{
				this->cells[index]->_hoverColor = this->selectedHoverColor;
				this->cells[index]->_pushedColor = this->selectedPushedColor;
			}
		}
	}

	void GridView::_updateScrollArea()
	{
		if (this->scrollArea != NULL)
		{
			float scrollOffsetY = this->scrollArea->getScrollOffsetY();
			this->scrollArea->setHeight(this->rows.size() * this->itemHeight + (this->rows.size() - 1) * this->spacingHeight);
			this->scrollArea->setScrollOffsetY(scrollOffsetY);
			this->scrollArea->setVisible(this->rows.size() > 0);
		}
	}

	GridViewRow* GridView::createRow(int index, chstr name)
	{
		if (this->scrollArea == NULL)
		{
			hlog::errorf(aprilui::logTag, "Cannot create row at index '%d' in GridView '%s', no internal ScrollArea is present!", index, this->name.c_str());
			return NULL;
		}
		if (this->rowTemplate == NULL)
		{
			hlog::errorf(aprilui::logTag, "Cannot create row at index '%d' in GridView '%s', no RowTemplate defined!", index, this->name.c_str());
			return NULL;
		}
		GridViewCell* selected = this->getSelected();
		this->setSelectedIndex(-1);
		index = hclamp(index, 0, this->rows.size());
		GridViewRow* row = this->rowTemplate->_createRow(name != "" ? name : april::generateName("aprilui::GridViewRow"));
		this->rows -= row;
		int rowsCount = this->rows.size();
		this->rows.insert_at(index, row);
		int cellsCount = this->rowTemplate->_gridViewCells.size();
		// reordering cells/cells
		this->cells = this->cells(0, index * cellsCount) + this->cells(rowsCount * cellsCount, cellsCount) + this->cells(index * cellsCount, (rowsCount - index) * cellsCount);
		if (selected != NULL)
		{
			this->setSelectedIndex(this->cells.index_of(selected));
		}
		this->_updateDisplay();
		return row;
	}

	bool GridView::deleteRow(int index)
	{
		if (!hbetweenIE(index, 0, this->rows.size()))
		{
			hlog::warnf(aprilui::logTag, "Cannot delete row at index '%d' in GridView '%s', it does not exist!", index, this->name.c_str());
			return false;
		}
		GridViewCell* selected = this->getSelected();
		this->setSelectedIndex(-1);
		if (selected != NULL && selected->_gridViewRow == this->rows[index])
		{
			int cellIndex = this->cells.index_of(selected);
			int columnCount = this->rowTemplate->_gridViewCells.size();
			if (this->rows.size() == 1)
			{
				selected = NULL;
			}
			else if (index == this->rows.size() - 1)
			{
				selected = this->cells[hclamp(cellIndex - columnCount, 0, this->cells.size() - 2)];
			}
			else
			{
				selected = this->cells[hclamp(cellIndex + columnCount, 0, this->cells.size() - 2)];
			}
		}
		GridViewRow* row = this->rows.remove_at(index);
		this->cells -= row->_gridViewCells;
		this->dataset->destroyObjects(row);
		if (selected != NULL)
		{
			this->setSelectedIndex(this->cells.index_of(selected));
		}
		this->_updateDisplay();
		return true;
	}

	GridViewRow* GridView::getRowAt(int index)
	{
		return (hbetweenIE(index, 0, this->rows.size()) ? this->rows[index] : NULL);
	}

	GridViewCell* GridView::getItemAt(int index)
	{
		return (hbetweenIE(index, 0, this->cells.size()) ? this->cells[index] : NULL);
	}

	hstr GridView::getProperty(chstr name)
	{
		if (name == "spacing_width")	return this->getSpacingWidth();
		if (name == "spacing_height")	return this->getSpacingHeight();
		return SelectionContainer::getProperty(name);
	}

	bool GridView::setProperty(chstr name, chstr value)
	{
		if		(name == "spacing_width")	this->setSpacingWidth(value);
		else if (name == "spacing_height")	this->setSpacingHeight(value);
		else return SelectionContainer::setProperty(name, value);
		return true;
	}

}

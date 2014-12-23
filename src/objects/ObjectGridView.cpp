/// @file
/// @version 4.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

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
		this->evenColor.set(april::Color::Black, 128);
		this->oddColor.set(april::Color::DarkGrey, 128);
		this->spacingWidth = 4.0f;
		this->spacingHeight = 4.0f;
		this->rowTemplate = NULL;
	}

	GridView::GridView(const GridView& other) : SelectionContainer(other)
	{
		this->evenColor = other.evenColor;
		this->oddColor = other.oddColor;
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
			GridView::_propertyDescriptions += PropertyDescription("even_color", PropertyDescription::HEXCOLOR);
			GridView::_propertyDescriptions += PropertyDescription("odd_color", PropertyDescription::HEXCOLOR);
			GridView::_propertyDescriptions += PropertyDescription("spacing_width", PropertyDescription::FLOAT);
			GridView::_propertyDescriptions += PropertyDescription("spacing_height", PropertyDescription::FLOAT);
		}
		return (SelectionContainer::getPropertyDescriptions() + GridView::_propertyDescriptions);
	}

	void GridView::setEvenColor(april::Color value)
	{
		if (this->evenColor != value)
		{
			this->evenColor = value;
			this->_updateDisplay();
		}
	}

	void GridView::setOddColor(april::Color value)
	{
		if (this->oddColor != value)
		{
			this->oddColor = value;
			this->_updateDisplay();
		}
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

	GridViewRow* GridView::getSelected()
	{
		return (is_between_ie(this->selectedIndex, 0, this->items.size()) ? this->items[this->selectedIndex] : NULL);
	}

	int GridView::getItemCount()
	{
		return this->cells.size();
	}

	void GridView::_updateDisplay()
	{
		for_iter (i, 0, this->cells.size())
		{
			this->_updateItem(i);
		}
		this->_updateScrollArea();
	}

	void GridView::_updateRow(int index)
	{
		if (is_between_ie(index, 0, this->items.size()))
		{
			this->items[index]->setY(index * (this->itemHeight + this->spacingHeight));
			this->items[index]->setHeight(this->itemHeight);
		}
	}

	void GridView::_updateItem(int index)
	{
		if (is_between_ie(index, 0, this->cells.size()))
		{
			if (this->selectedIndex != index)
			{
				this->cells[index]->_backColor = (index % 2 == 0 ? this->evenColor : this->oddColor);
				this->cells[index]->_hoverColor = this->hoverColor;
				this->cells[index]->_pushedColor = this->pushedColor;
			}
			else
			{
				this->cells[index]->_backColor = this->selectedColor;
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
			this->scrollArea->setHeight(this->items.size() * this->itemHeight + (this->items.size() - 1) * this->spacingHeight);
			this->scrollArea->setScrollOffsetY(scrollOffsetY);
			this->scrollArea->setVisible(this->items.size() > 0);
		}
	}

	/*
	GridViewRow* GridView::createItem(int index, chstr name)
	{
		if (this->scrollArea == NULL)
		{
			hlog::errorf(aprilui::logTag, "Cannot create item at index '%d' in GridView '%s', no internal ScrollArea is present!", index, this->name.c_str());
			return NULL;
		}
		GridViewRow* selected = this->getSelected();
		this->setSelectedIndex(-1);
		index = hclamp(index, 0, this->items.size());
		GridViewRow* item = new GridViewRow(name != "" ? name : april::generateName("aprilui::GridViewRow"));
		this->registerChild(item);
		this->items -= item;
		this->items.insert_at(index, item);
		if (selected != NULL)
		{
			this->setSelectedIndex(this->items.index_of(selected));
		}
		this->_updateDisplay();
		return item;
	}

	bool GridView::deleteItem(int index)
	{
		if (!is_between_ie(index, 0, this->items.size()))
		{
			hlog::warnf(aprilui::logTag, "Cannot delete item at index '%d' in GridView '%s', it does not exist!", index, this->name.c_str());
			return false;
		}
		GridViewRow* selected = this->getSelected();
		this->setSelectedIndex(-1);
		if (selected != NULL && selected == this->items[index])
		{
			selected = this->items[hclamp(index - 1, 0, this->items.size() - 2)];
		}
		this->dataset->destroyObjects(this->items.remove_at(index));
		if (selected != NULL)
		{
			this->setSelectedIndex(this->items.index_of(selected));
		}
		this->_updateDisplay();
		return true;
	}

	GridViewRow* GridView::getItemAt(int index)
	{
		return (is_between_ie(index, 0, this->items.size()) ? this->items[index] : NULL);
	}
	*/

	hstr GridView::getProperty(chstr name)
	{
		if (name == "even_color")		return this->getEvenColor().hex();
		if (name == "odd_color")		return this->getOddColor().hex();
		if (name == "spacing_width")	return this->getSpacingWidth();
		if (name == "spacing_height")	return this->getSpacingHeight();
		return SelectionContainer::getProperty(name);
	}

	bool GridView::setProperty(chstr name, chstr value)
	{
		if		(name == "even_color")		this->setEvenColor(value);
		else if (name == "odd_color")		this->setOddColor(value);
		else if (name == "spacing_width")	this->setSpacingWidth(value);
		else if (name == "spacing_height")	this->setSpacingHeight(value);
		else return SelectionContainer::setProperty(name, value);
		return true;
	}

}

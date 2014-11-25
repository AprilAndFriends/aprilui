/// @file
/// @version 3.6
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
#include "ObjectListBox.h"
#include "ObjectListBoxItem.h"
#include "ObjectScrollArea.h"
#include "ObjectScrollBarV.h"

namespace aprilui
{
	harray<PropertyDescription> ListBox::_propertyDescriptions;

	ListBox::ListBox(chstr name) : Container(name)
	{
		this->itemHeight = 32.0f;
		this->evenColor.set(april::Color::Black, 128);
		this->oddColor.set(april::Color::DarkGrey, 128);
		this->selectedColor.set(april::Color::White, 128);
		this->selectedIndex = -1;
		// setup ScrollArea
		this->registerChild(new ScrollArea(april::generateName("aprilui::ScrollArea"))); // sets this->scrollArea
		this->scrollArea->setRect(this->rect);
		this->scrollArea->setAnchors(true, true, true, false);
		this->scrollArea->setVisible(false);
	}

	ListBox::~ListBox()
	{
	}

	Object* ListBox::createInstance(chstr name)
	{
		return new ListBox(name);
	}

	harray<PropertyDescription> ListBox::getPropertyDescriptions()
	{
		if (ListBox::_propertyDescriptions.size() == 0)
		{
			ListBox::_propertyDescriptions += PropertyDescription("item_height", PropertyDescription::FLOAT);
			ListBox::_propertyDescriptions += PropertyDescription("selected_index", PropertyDescription::INT);
			ListBox::_propertyDescriptions += PropertyDescription("even_color", PropertyDescription::HEXCOLOR);
			ListBox::_propertyDescriptions += PropertyDescription("odd_color", PropertyDescription::HEXCOLOR);
			ListBox::_propertyDescriptions += PropertyDescription("hover_color", PropertyDescription::HEXCOLOR);
			ListBox::_propertyDescriptions += PropertyDescription("selected_color", PropertyDescription::HEXCOLOR);
			ListBox::_propertyDescriptions += PropertyDescription("item_count", PropertyDescription::INT);
		}
		return (Container::getPropertyDescriptions() + ListBox::_propertyDescriptions);
	}

	void ListBox::setItemHeight(float value)
	{
		if (this->itemHeight != value)
		{
			this->itemHeight = value;
			this->_updateSystem();
		}
	}

	void ListBox::setSelectedIndex(int value)
	{
		if (this->selectedIndex != value)
		{
			int oldIndex = this->selectedIndex;
			this->selectedIndex = (value < this->items.size() ? value : -1);
			if (this->selectedIndex != oldIndex)
			{
				this->_updateItem(oldIndex);
				this->_updateItem(this->selectedIndex);
			}
		}
	}

	void ListBox::setEvenColor(april::Color value)
	{
		if (this->evenColor != value)
		{
			this->evenColor = value;
			this->_updateItems();
		}
	}

	void ListBox::setOddColor(april::Color value)
	{
		if (this->oddColor != value)
		{
			this->oddColor = value;
			this->_updateItems();
		}
	}

	void ListBox::setHoverColor(april::Color value)
	{
		if (this->hoverColor != value)
		{
			this->hoverColor = value;
			this->_updateItems();
		}
	}

	void ListBox::setSelectedColor(april::Color value)
	{
		if (this->selectedColor != value)
		{
			this->selectedColor = value;
			this->_updateItems();
		}
	}

	int ListBox::getItemCount()
	{
		return this->items.size();
	}

	void ListBox::_updateSystem()
	{
		this->_updateItems();
		this->_updateScrollArea();
	}

	void ListBox::_updateItems()
	{
		for_iter (i, 0, this->items.size())
		{
			this->_updateItem(i);
		}
	}

	void ListBox::_updateItem(int index)
	{
		if (is_between_ie(index, 0, this->items.size()))
		{
			this->items[index]->setY(index * this->itemHeight);
			this->items[index]->setHeight(this->itemHeight);
			if (this->selectedIndex != index)
			{
				this->items[index]->setBackgroundColor(index % 2 == 0 ? this->evenColor : this->oddColor);
			}
			else
			{
				this->items[index]->setBackgroundColor(this->selectedColor);
			}
		}
	}

	void ListBox::_updateScrollArea()
	{
		if (this->scrollArea != NULL)
		{
			float scrollOffsetY = this->scrollArea->getScrollOffsetY();
			this->scrollArea->setHeight(this->items.size() * this->itemHeight);
			this->scrollArea->setScrollOffsetY(scrollOffsetY);
		}
	}

	ListBoxItem* ListBox::createItem(int index)
	{
		if (this->scrollArea == NULL)
		{
			hlog::errorf(aprilui::logTag, "Cannot create item at index '%d' in ListBox '%s', no internal ScrollArea is present!", index, this->name.c_str());
			return NULL;
		}
		index = hclamp(index, 0, this->items.size());
		ListBoxItem* item = new ListBoxItem(april::generateName("aprilui::ListBoxItem"));
		item->setRect(grect(0.0f, index * this->itemHeight, this->rect.w, this->itemHeight));
		item->setAnchors(true, true, true, false);
		item->setBackgroundBorder(false);
		this->scrollArea->registerChild(item);
		this->items.insert_at(index, item);
		this->_updateItems();
		this->_updateScrollArea();
		return item;
	}

	bool ListBox::deleteItem(int index)
	{
		if (!is_between_ie(index, 0, this->items.size()))
		{
			hlog::warnf(aprilui::logTag, "Cannot delete item at index '%d' in ListBox '%s', it does not exist!", index, this->name.c_str());
			return false;
		}
		this->scrollArea->unregisterChild(this->items[index]);
		delete this->items.remove_at(index);
		this->_updateItems();
		this->_updateScrollArea();
		return true;
	}

	hstr ListBox::getProperty(chstr name)
	{
		if (name == "item_height")		return this->getItemHeight();
		if (name == "selected_index")	return this->getSelectedIndex();
		if (name == "even_color")		return this->getEvenColor().hex();
		if (name == "odd_color")		return this->getOddColor().hex();
		if (name == "hover_color")		return this->getHoverColor().hex();
		if (name == "selected_color")	return this->getSelectedColor().hex();
		if (name == "item_count")		return this->getItemCount();
		return Container::getProperty(name);
	}

	bool ListBox::setProperty(chstr name, chstr value)
	{
		if (name == "item_height")			this->setItemHeight(value);
		else if (name == "selected_index")	this->setSelectedIndex(value);
		else if (name == "even_color")		this->setEvenColor(value);
		else if (name == "odd_color")		this->setOddColor(value);
		else if (name == "hover_color")		this->setHoverColor(value);
		else if (name == "selected_color")	this->setSelectedColor(value);
		else return Container::setProperty(name, value);
		return true;
	}

}

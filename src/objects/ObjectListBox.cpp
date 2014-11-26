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
		this->hoverColor.set(april::Color::White, 192);
		this->pushedColor.set(april::Color::LightGrey, 192);
		this->selectedColor.set(april::Color::Aqua, 192);
		this->selectedHoverColor.set(april::Color::LightAqua, 192);
		this->selectedPushedColor.set(april::Color::DarkAqua, 192);
		this->selectedIndex = -1;
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
			ListBox::_propertyDescriptions += PropertyDescription("selected_hover_color", PropertyDescription::HEXCOLOR);
			ListBox::_propertyDescriptions += PropertyDescription("selected_pushed_color", PropertyDescription::HEXCOLOR);
			ListBox::_propertyDescriptions += PropertyDescription("item_count", PropertyDescription::INT);
		}
		return (Container::getPropertyDescriptions() + ListBox::_propertyDescriptions);
	}

	void ListBox::setItemHeight(float value)
	{
		if (this->itemHeight != value)
		{
			this->itemHeight = value;
			this->_updateDisplay();
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
				this->triggerEvent(Event::SelectedChanged, hstr(this->selectedIndex));
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

	void ListBox::setPushedColor(april::Color value)
	{
		if (this->pushedColor != value)
		{
			this->pushedColor = value;
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

	void ListBox::setSelectedHoverColor(april::Color value)
	{
		if (this->selectedHoverColor != value)
		{
			this->selectedHoverColor = value;
			this->_updateItems();
		}
	}

	void ListBox::setSelectedPushedColor(april::Color value)
	{
		if (this->selectedPushedColor != value)
		{
			this->selectedPushedColor = value;
			this->_updateItems();
		}
	}

	int ListBox::getItemCount()
	{
		return this->items.size();
	}

	void ListBox::_updateDisplay()
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
				this->items[index]->_hoverColor = this->hoverColor;
				this->items[index]->_pushedColor = this->pushedColor;
			}
			else
			{
				this->items[index]->setBackgroundColor(this->selectedColor);
				this->items[index]->_hoverColor = this->selectedHoverColor;
				this->items[index]->_pushedColor = this->selectedPushedColor;
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

	ListBoxItem* ListBox::createItem(int index, chstr name)
	{
		if (this->scrollArea == NULL)
		{
			hlog::errorf(aprilui::logTag, "Cannot create item at index '%d' in ListBox '%s', no internal ScrollArea is present!", index, this->name.c_str());
			return NULL;
		}
		index = hclamp(index, 0, this->items.size());
		ListBoxItem* item = new ListBoxItem(name != "" ? name : april::generateName("aprilui::ListBoxItem"));
		item->_listBox = this;
		this->scrollArea->registerChild(item);
		this->items.insert_at(index, item);
		item->setRect(grect(0.0f, index * this->itemHeight, this->rect.w, this->itemHeight));
		item->setAnchors(true, true, true, false);
		item->setBackgroundBorder(false);
		item->_hoverColor = this->hoverColor;
		item->_pushedColor = this->pushedColor;
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
		if (this->selectedIndex >= 0)
		{
			this->setSelectedIndex(this->items.size() > 0 ? hclamp(this->selectedIndex, 0, this->items.size() - 1) : -1); // has to refresh the display
		}
		this->_updateItems();
		this->_updateScrollArea();
		return true;
	}

	ListBoxItem* ListBox::getItemAt(int index)
	{
		return (is_between_ie(index, 0, this->items.size()) ? this->items[index] : NULL);
	}

	hstr ListBox::getProperty(chstr name)
	{
		if (name == "item_height")				return this->getItemHeight();
		if (name == "selected_index")			return this->getSelectedIndex();
		if (name == "even_color")				return this->getEvenColor().hex();
		if (name == "odd_color")				return this->getOddColor().hex();
		if (name == "hover_color")				return this->getHoverColor().hex();
		if (name == "selected_color")			return this->getSelectedColor().hex();
		if (name == "selected_hover_color")		return this->getSelectedHoverColor().hex();
		if (name == "selected_pushed_color")	return this->getSelectedPushedColor().hex();
		if (name == "item_count")				return this->getItemCount();
		return Container::getProperty(name);
	}

	bool ListBox::setProperty(chstr name, chstr value)
	{
		if (name == "item_height")					this->setItemHeight(value);
		else if (name == "selected_index")			this->setSelectedIndex(value);
		else if (name == "even_color")				this->setEvenColor(value);
		else if (name == "odd_color")				this->setOddColor(value);
		else if (name == "hover_color")				this->setHoverColor(value);
		else if (name == "selected_color")			this->setSelectedColor(value);
		else if (name == "selected_hover_color")	this->setSelectedHoverColor(value);
		else if (name == "selected_pushed_color")	this->setSelectedPushedColor(value);
		else return Container::setProperty(name, value);
		return true;
	}

	void ListBox::notifyEvent(chstr type, EventArgs* args)
	{
		Container::notifyEvent(type, args);
		if (type == Event::RegisteredInDataset)
		{
			if (this->scrollArea == NULL)
			{
				this->registerChild(new ScrollArea(april::generateName("aprilui::ScrollArea"))); // sets this->scrollArea
				this->scrollArea->setRect(this->rect);
				this->scrollArea->setAnchors(true, true, true, false);
				this->scrollArea->setVisible(false);
			}
		}
	}

}

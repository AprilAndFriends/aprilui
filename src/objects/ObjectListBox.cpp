/// @file
/// @version 5.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <gtypes/Rectangle.h>
#include <hltypes/hlog.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "apriluiUtil.h"
#include "Dataset.h"
#include "ObjectListBox.h"
#include "ObjectListBoxItem.h"
#include "ObjectScrollArea.h"
#include "ObjectScrollBarV.h"

namespace aprilui
{
	hmap<hstr, PropertyDescription> ListBox::_propertyDescriptions;
	hmap<hstr, PropertyDescription::Accessor*> ListBox::_getters;
	hmap<hstr, PropertyDescription::Accessor*> ListBox::_setters;

	ListBox::ListBox(chstr name) :
		SelectionContainer(name)
	{
		this->evenColor.set(april::Color::Black, 128);
		this->oddColor.set(april::Color::DarkGrey, 128);
	}

	ListBox::ListBox(const ListBox& other) :
		SelectionContainer(other)
	{
		this->evenColor = other.evenColor;
		this->oddColor = other.oddColor;
	}

	ListBox::~ListBox()
	{
	}

	Object* ListBox::createInstance(chstr name)
	{
		return new ListBox(name);
	}

	hmap<hstr, PropertyDescription>& ListBox::getPropertyDescriptions() const
	{
		if (ListBox::_propertyDescriptions.size() == 0)
		{
			ListBox::_propertyDescriptions = SelectionContainer::getPropertyDescriptions();
			ListBox::_propertyDescriptions["even_color"] = PropertyDescription("even_color", PropertyDescription::Type::Color);
			ListBox::_propertyDescriptions["odd_color"] = PropertyDescription("odd_color", PropertyDescription::Type::Color);
		}
		return ListBox::_propertyDescriptions;
	}

	hmap<hstr, PropertyDescription::Accessor*>& ListBox::_getGetters() const
	{
		if (ListBox::_getters.size() == 0)
		{
			ListBox::_getters = SelectionContainer::_getGetters();
			ListBox::_getters["even_color"] = new PropertyDescription::GetColor<ListBox>(&ListBox::getEvenColor);
			ListBox::_getters["odd_color"] = new PropertyDescription::GetColor<ListBox>(&ListBox::getOddColor);
		}
		return ListBox::_getters;
	}

	hmap<hstr, PropertyDescription::Accessor*>& ListBox::_getSetters() const
	{
		if (ListBox::_setters.size() == 0)
		{
			ListBox::_setters = SelectionContainer::_getSetters();
			ListBox::_setters["even_color"] = new PropertyDescription::Set<ListBox, hstr>(&ListBox::setEvenSymbolicColor);
			ListBox::_setters["odd_color"] = new PropertyDescription::Set<ListBox, hstr>(&ListBox::setOddSymbolicColor);
		}
		return ListBox::_setters;
	}

	void ListBox::setEvenColor(const april::Color& value)
	{
		if (this->evenColor != value)
		{
			this->evenColor = value;
			this->_updateDisplay();
		}
	}

	void ListBox::setEvenSymbolicColor(chstr value)
	{
		this->setEvenColor(aprilui::_makeColor(value));
	}

	void ListBox::setOddColor(const april::Color& value)
	{
		if (this->oddColor != value)
		{
			this->oddColor = value;
			this->_updateDisplay();
		}
	}

	void ListBox::setOddSymbolicColor(chstr value)
	{
		this->setOddColor(aprilui::_makeColor(value));
	}

	ListBoxItem* ListBox::getSelected() const
	{
		return (hbetweenIE(this->selectedIndex, 0, this->items.size()) ? this->items[this->selectedIndex] : NULL);
	}

	int ListBox::getItemCount() const
	{
		return this->items.size();
	}

	void ListBox::_updateDisplay()
	{
		for_iter (i, 0, this->items.size())
		{
			this->items[i]->setVisible(true);
			this->_updateItem(i);
		}
		this->_updateScrollArea();
	}

	void ListBox::_updateItem(int index)
	{
		if (hbetweenIE(index, 0, this->items.size()))
		{
			this->items[index]->setY(index * this->itemHeight);
			this->items[index]->setHeight(this->itemHeight);
			if (this->selectedIndex != index)
			{
				this->items[index]->_backColor = (index % 2 == 0 ? this->evenColor : this->oddColor);
				this->items[index]->_hoverColor = this->hoverColor;
				this->items[index]->_pushedColor = this->pushedColor;
			}
			else
			{
				this->items[index]->_backColor = this->selectedColor;
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
			this->scrollArea->setVisible(this->items.size() > 0);
			this->_optimizeVisibility();
		}
	}

	void ListBox::_optimizeVisibility()
	{
		if (this->scrollArea != NULL)
		{
			grectf rect(this->scrollArea->getScrollOffset(), this->scrollArea->getParent()->getSize());
			foreach (ListBoxItem*, it, this->items)
			{
				if (rect.intersects((*it)->getRect()))
				{
					(*it)->setVisible(true);
					(*it)->setAwake(true);
				}
				else
				{
					(*it)->setVisible(false);
					(*it)->setAwake(false);
				}
			}
		}
	}

	ListBoxItem* ListBox::createItem(int index, chstr name)
	{
		if (this->scrollArea == NULL)
		{
			hlog::errorf(logTag, "Cannot create item at index '%d' in ListBox '%s', no internal ScrollArea is present!", index, this->name.cStr());
			return NULL;
		}
		ListBoxItem* selected = this->getSelected();
		this->setSelectedIndex(-1);
		index = hclamp(index, 0, this->items.size());
		ListBoxItem* item = new ListBoxItem(name != "" ? name : april::generateName("aprilui::ListBoxItem"));
		this->registerChild(item);
		this->items -= item;
		this->items.insertAt(index, item);
		if (selected != NULL)
		{
			this->setSelectedIndex(this->items.indexOf(selected));
		}
		this->_updateDisplay();
		return item;
	}

	bool ListBox::deleteItem(int index)
	{
		if (!hbetweenIE(index, 0, this->items.size()))
		{
			hlog::warnf(logTag, "Cannot delete item at index '%d' in ListBox '%s', it does not exist!", index, this->name.cStr());
			return false;
		}
		ListBoxItem* selected = this->getSelected();
		this->setSelectedIndex(-1);
		if (selected != NULL && selected == this->items[index])
		{
			selected = this->items[hclamp(index + 1, 0, this->items.size() - 2)];
		}
		this->dataset->destroyObjects(this->items.removeAt(index));
		if (selected != NULL)
		{
			this->setSelectedIndex(this->items.indexOf(selected));
		}
		this->_updateDisplay();
		return true;
	}

	ListBoxItem* ListBox::getItemAt(int index)
	{
		return (hbetweenIE(index, 0, this->items.size()) ? this->items[index] : NULL);
	}

	void ListBox::snapToSelectedItem()
	{
		if (this->selectedIndex >= 0)
		{
			this->scrollArea->setScrollOffsetY(hclamp(this->scrollArea->getScrollOffsetY(), (this->selectedIndex + 1) * this->itemHeight - this->rect.h, this->selectedIndex * this->itemHeight));
			this->_optimizeVisibility();
		}
	}

}

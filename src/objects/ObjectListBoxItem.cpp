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
#include "ObjectListBox.h"
#include "ObjectListBoxItem.h"
#include "ObjectScrollArea.h"

namespace aprilui
{
	harray<PropertyDescription> ListBoxItem::_propertyDescriptions;

	ListBoxItem::ListBoxItem(chstr name) : Label(name), ButtonBase()
	{
		this->_listBox = NULL;
		this->_backColor = april::Color::Clear;
		this->_hoverColor.set(april::Color::White, 192);
		this->_pushedColor.set(april::Color::LightGrey, 192);
	}

	ListBoxItem::ListBoxItem(const ListBoxItem& other) : Label(other), ButtonBase(other)
	{
		this->_listBox = NULL;
		this->_backColor = other._backColor;
		this->_hoverColor = other._hoverColor;
		this->_pushedColor = other._pushedColor;
	}

	ListBoxItem::~ListBoxItem()
	{
	}

	Object* ListBoxItem::createInstance(chstr name)
	{
		return new ListBoxItem(name);
	}

	harray<PropertyDescription> ListBoxItem::getPropertyDescriptions() const
	{
		if (ListBoxItem::_propertyDescriptions.size() == 0)
		{
			ListBoxItem::_propertyDescriptions = Label::getPropertyDescriptions() + ButtonBase::getPropertyDescriptions();
		}
		return ListBoxItem::_propertyDescriptions;
	}

	hstr ListBoxItem::getName() const
	{
		return Label::getName();
	}

	int ListBoxItem::getFocusIndex() const
	{
		return Label::getFocusIndex();
	}

	Object* ListBoxItem::getParent() const
	{
		return Label::getParent();
	}

	Dataset* ListBoxItem::getDataset() const
	{
		return Label::getDataset();
	}

	bool ListBoxItem::isCursorInside() const
	{
		return Label::isCursorInside();
	}

	bool ListBoxItem::isSelected() const
	{
		return (this->_listBox != NULL && hbetweenIE(this->_listBox->selectedIndex, 0, this->_listBox->items.size()) &&
			this->_listBox->items[this->_listBox->selectedIndex] == this);
	}

	void ListBoxItem::_update(float timeDelta)
	{
		ButtonBase::_update(timeDelta);
		Label::_update(timeDelta);
	}

	void ListBoxItem::notifyEvent(chstr type, EventArgs* args)
	{
		Label::notifyEvent(type, args);
		if (type == Event::AttachedToObject)
		{
			ListBox* listBox = dynamic_cast<ListBox*>(this->parent);
			if (listBox != NULL)
			{
				this->_listBox = listBox;
				if (this->_listBox->scrollArea != NULL)
				{
					int itemCount = this->_listBox->getItemCount();
					float itemHeight = this->_listBox->getItemHeight();
					// reattach to ScrollArea
					this->_listBox->removeChild(this);
					this->_listBox->scrollArea->addChild(this);
					// setup all properties
					this->_listBox->items += this;
					this->setRect(0.0f, itemCount * itemHeight, this->_listBox->getWidth(), itemHeight);
					this->setAnchors(true, true, true, false);
					this->setBackgroundBorder(false);
					this->_hoverColor = this->_listBox->hoverColor;
					this->_pushedColor = this->_listBox->pushedColor;
					this->_listBox->_updateItem(itemCount);
					this->_listBox->_updateScrollArea();
				}
				else
				{
					hlog::errorf(logTag, "ListBoxItem '%s' cannot be reattached to ScrollArea of ListBox '%s', ScrollArea does not exist!", this->name.cStr(), this->parent->getName().cStr());
				}
			}
			else if (this->parent != NULL && dynamic_cast<ScrollArea*>(this->parent) == NULL)
			{
				this->_listBox = NULL;
				hlog::errorf(logTag, "ListBoxItem '%s' not attached to object of class ListBox!", this->name.cStr());
			}
		}
	}

	bool ListBoxItem::triggerEvent(chstr type, april::Key keyCode)
	{
		return Label::triggerEvent(type, keyCode);
	}

	bool ListBoxItem::triggerEvent(chstr type, april::Key keyCode, chstr string)
	{
		return Label::triggerEvent(type, keyCode, string);
	}

	bool ListBoxItem::triggerEvent(chstr type, april::Key keyCode, cgvec2 position, chstr string, void* userData)
	{
		return Label::triggerEvent(type, keyCode, position, string, userData);
	}

	bool ListBoxItem::triggerEvent(chstr type, april::Button buttonCode, chstr string, void* userData)
	{
		return Label::triggerEvent(type, buttonCode, string, userData);
	}

	bool ListBoxItem::triggerEvent(chstr type, chstr string, void* userData)
	{
		return Label::triggerEvent(type, string, userData);
	}

	bool ListBoxItem::triggerEvent(chstr type, void* userData)
	{
		return Label::triggerEvent(type, userData);
	}

	void ListBoxItem::_draw()
	{
		april::Color drawColor = this->_makeDrawColor();
		if (this->_listBox != NULL)
		{
			april::Color color = this->_getCurrentBackgroundColor() * drawColor;
			if (color.a > 0)
			{
				april::rendersys->setBlendMode(april::BlendMode::Alpha);
				april::rendersys->setColorMode(april::ColorMode::Multiply);
				april::rendersys->drawFilledRect(this->_makeDrawRect(), color);
			}
		}
		Label::_draw();
	}

	april::Color ListBoxItem::_getCurrentBackgroundColor() const
	{
		if (this->_listBox != NULL)
		{
			if (this->pushed)
			{
				return (!this->isSelected() ? this->_listBox->getPushedColor() : this->_listBox->getSelectedPushedColor());
			}
			if (this->hovered)
			{
				return (!this->isSelected() ? this->_listBox->getHoverColor() : this->_listBox->getSelectedHoverColor());
			}
			if (this->isSelected())
			{
				return this->_listBox->getSelectedColor();
			}
		}
		return this->_backColor;
	}

	void ListBoxItem::_setSelected()
	{
		if (this->_listBox != NULL)
		{
			this->_listBox->setSelectedIndex(this->_listBox->items.indexOf(this));
		}
	}

	bool ListBoxItem::_mouseDown(april::Key keyCode)
	{
		bool result = ButtonBase::_mouseDown(keyCode);
		if (result)
		{
			this->triggerEvent(Event::MouseDown, keyCode);
		}
		return (result || Label::_mouseDown(keyCode));
	}

	bool ListBoxItem::_mouseUp(april::Key keyCode)
	{
		bool result = ButtonBase::_mouseUp(keyCode);
		bool up = false;
		if (this->hovered)
		{
			up = this->triggerEvent(Event::MouseUp, keyCode);
		}
		if (result)
		{
			this->_setSelected();
			this->triggerEvent(Event::Click, keyCode);
		}
		return (result || up || Label::_mouseUp(keyCode));
	}

	void ListBoxItem::_mouseCancel(april::Key keyCode)
	{
		ButtonBase::_mouseCancel(keyCode);
		Label::_mouseCancel(keyCode);
	}

	bool ListBoxItem::_mouseMove()
	{
		return (ButtonBase::_mouseMove() || Label::_mouseMove());
	}

	bool ListBoxItem::_buttonDown(april::Button buttonCode)
	{
		bool result = ButtonBase::_buttonDown(buttonCode);
		if (result)
		{
			this->triggerEvent(Event::ButtonDown, buttonCode);
		}
		return (result || Label::_buttonDown(buttonCode));
	}

	bool ListBoxItem::_buttonUp(april::Button buttonCode)
	{
		if (Label::_buttonUp(buttonCode))
		{
			return true;
		}
		bool result = ButtonBase::_buttonUp(buttonCode);
		bool up = false;
		if (this->hovered)
		{
			up = this->triggerEvent(Event::ButtonUp, buttonCode);
		}
		if (result)
		{
			this->_setSelected();
			this->triggerEvent(Event::ButtonTrigger, buttonCode);
		}
		return (result || up || Label::_buttonUp(buttonCode));
	}

}

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
#include "ObjectListBox.h"
#include "ObjectListBoxItem.h"
#include "ObjectScrollArea.h"

namespace aprilui
{
	ListBoxItem::ListBoxItem(chstr name) : Label(name), ButtonBase()
	{
		this->_listBox = NULL;
		this->_hoverColor.set(april::Color::White, 192);
		this->_pushedColor.set(april::Color::LightGrey, 192);
	}

	ListBoxItem::~ListBoxItem()
	{
	}

	Object* ListBoxItem::createInstance(chstr name)
	{
		return new ListBoxItem(name);
	}

	hstr ListBoxItem::getName()
	{
		return Label::getName();
	}

	int ListBoxItem::getFocusIndex()
	{
		return Label::getFocusIndex();
	}

	Object* ListBoxItem::getParent()
	{
		return Label::getParent();
	}

	Dataset* ListBoxItem::getDataset()
	{
		return Label::getDataset();
	}

	bool ListBoxItem::isCursorInside()
	{
		return Label::isCursorInside();
	}

	void ListBoxItem::update(float timeDelta)
	{
		ButtonBase::update(timeDelta);
		Label::update(timeDelta);
	}

	bool ListBoxItem::triggerEvent(chstr type, april::Key keyCode)
	{
		return Label::triggerEvent(type, keyCode);
	}

	bool ListBoxItem::triggerEvent(chstr type, april::Key keyCode, chstr string)
	{
		return Label::triggerEvent(type, keyCode, string);
	}

	bool ListBoxItem::triggerEvent(chstr type, april::Key keyCode, gvec2 position, chstr string, void* userData)
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

	void ListBoxItem::notifyEvent(chstr type, EventArgs* args)
	{
		Label::notifyEvent(type, args);
		if (type == Event::AttachedToObject)
		{
			ListBox* listBox = dynamic_cast<ListBox*>(this->parent);
			if (listBox != NULL)
			{
				this->_listBox = listBox;
				ScrollArea* scrollArea = this->_listBox->_getScrollArea();
				if (scrollArea != NULL)
				{
					int itemCount = this->_listBox->getItemCount();
					float itemHeight = this->_listBox->getItemHeight();
					// reattach to other ScrollArea
					this->_listBox->removeChild(this);
					scrollArea->addChild(this);
					scrollArea->setVisible(true);
					// setup all properties
					this->_listBox->items += this;
					this->setRect(grect(0.0f, itemCount * itemHeight, this->_listBox->getWidth(), itemHeight));
					this->setAnchors(true, true, true, false);
					this->setBackgroundBorder(false);
					this->_hoverColor = this->_listBox->hoverColor;
					this->_pushedColor = this->_listBox->pushedColor;
					this->_listBox->_updateItem(itemCount);
					this->_listBox->_updateScrollArea();
				}
				else
				{
					hlog::warnf(aprilui::logTag, "ListBoxItem '%s' cannot be reattached to ScrollArea of ListBox '%s', ScrollArea does not exist!", this->name.c_str(), this->parent->getName().c_str());
				}
			}
			else if (this->parent != NULL && dynamic_cast<ScrollArea*>(this->parent) == NULL)
			{
				this->_listBox = NULL;
				hlog::warnf(aprilui::logTag, "ListBoxItem '%s' not attached to object of class ListBox!", this->name.c_str());
			}
		}
	}

	void ListBoxItem::_draw()
	{
		if (!this->hovered && !this->pushed)
		{
			Label::_draw();
		}
		else
		{
			april::Color backgroundColor = this->backgroundColor;
			this->backgroundColor = (!this->pushed ? this->_hoverColor : this->_pushedColor);
			Label::_draw();
			this->backgroundColor = backgroundColor;
		}
	}

	void ListBoxItem::_setSelected()
	{
		if (this->_listBox != NULL)
		{
			this->_listBox->setSelectedIndex(this->_listBox->items.index_of(this));
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
		if (Label::onButtonUp(buttonCode)) // not a mistake, ImageBox does handle a MouseUp even and this behavior has to be overriden (will be refactored)
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

	void ListBoxItem::_mouseCancel(april::Key keyCode)
	{
		ButtonBase::_mouseCancel(keyCode);
		Label::_mouseCancel(keyCode);
	}

}

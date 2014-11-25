/// @file
/// @version 3.6
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
	ListBoxItem::ListBoxItem(chstr name) : Label(name)
	{
	}

	ListBoxItem::~ListBoxItem()
	{
	}

	Object* ListBoxItem::createInstance(chstr name)
	{
		return new ListBoxItem(name);
	}

	void ListBoxItem::notifyEvent(chstr type, EventArgs* args)
	{
		Label::notifyEvent(type, args);
		if (type == Event::AttachedToObject)
		{
			ListBox* parent = dynamic_cast<ListBox*>(this->parent);
			if (parent != NULL)
			{
				ScrollArea* scrollArea = parent->_getScrollArea();
				if (scrollArea != NULL)
				{
					int itemCount = parent->getItemCount();
					float itemHeight = parent->getItemHeight();
					// reattach to other ScrollArea
					parent->removeChild(this);
					scrollArea->addChild(this);
					scrollArea->setVisible(true);
					// setup all properties
					parent->items += this;
					this->setRect(grect(0.0f, itemCount * itemHeight, parent->getWidth(), itemHeight));
					this->setAnchors(true, true, true, false);
					this->setBackgroundBorder(false);
					parent->_updateItem(itemCount);
					parent->_updateScrollArea();
				}
				else
				{
					hlog::warnf(aprilui::logTag, "ListBoxItem '%s' cannot be reattached to ScrollArea of ListBox '%s', ScrollArea does not exist!", this->name.c_str(), this->parent->getName().c_str());
				}
			}
			else if (this->parent != NULL && dynamic_cast<ScrollArea*>(this->parent) == NULL)
			{
				hlog::warnf(aprilui::logTag, "ListBoxItem '%s' not attached to object of class ListBox!", this->name.c_str());
			}
		}
	}

}

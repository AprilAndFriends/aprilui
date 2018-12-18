/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hlog.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "ObjectGridView.h"
#include "ObjectGridViewRow.h"
#include "ObjectGridViewRowTemplate.h"
#include "ObjectScrollArea.h"

namespace aprilui
{
	GridViewRow::GridViewRow(chstr name) :
		Container(name)
	{
		this->gridView = NULL;
	}

	GridViewRow::GridViewRow(const GridViewRow& other) :
		Container(other)
	{
		this->gridView = NULL;
	}

	Object* GridViewRow::createInstance(chstr name)
	{
		return new GridViewRow(name);
	}

	void GridViewRow::notifyEvent(chstr type, EventArgs* args)
	{
		Container::notifyEvent(type, args);
		if (type == Event::AttachedToObject)
		{
			GridView* gridView = dynamic_cast<GridView*>(this->parent);
			if (gridView != NULL)
			{
				this->gridView = gridView;
				if (this->gridView->scrollArea != NULL)
				{
					int itemCount = this->gridView->rows.size();
					float itemHeight = this->gridView->getItemHeight();
					// reattach to ScrollArea
					this->gridView->removeChild(this);
					this->gridView->scrollArea->addChild(this);
					// setup all properties
					this->gridView->rows += this;
					this->setRect(0.0f, itemCount * (itemHeight + this->gridView->getSpacingHeight()), this->gridView->getWidth(), itemHeight);
					this->setAnchors(true, true, true, false);
					this->gridView->_updateRow(itemCount);
					this->gridView->_updateScrollArea();
				}
				else
				{
					hlog::errorf(logTag, "GridViewRow '%s' cannot be reattached to ScrollArea of GridView '%s', ScrollArea does not exist!", this->name.cStr(), this->parent->getName().cStr());
				}
			}
			else if (this->parent != NULL && dynamic_cast<ScrollArea*>(this->parent) == NULL)
			{
				this->gridView = NULL;
				hlog::errorf(logTag, "GridViewRow '%s' not attached to object of class GridView!", this->name.cStr());
			}
		}
	}

}

/// @file
/// @version 4.0
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
	GridViewRow::GridViewRow(chstr name) : Container(name)
	{
		this->_gridView = NULL;
	}

	GridViewRow::GridViewRow(const GridViewRow& other) : Container(other)
	{
		this->_gridView = NULL;
	}

	GridViewRow::~GridViewRow()
	{
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
				this->_gridView = gridView;
				if (this->_gridView->scrollArea != NULL)
				{
					int itemCount = this->_gridView->rows.size();
					float itemHeight = this->_gridView->getItemHeight();
					// reattach to ScrollArea
					this->_gridView->removeChild(this);
					this->_gridView->scrollArea->addChild(this);
					// setup all properties
					this->_gridView->rows += this;
					this->setRect(0.0f, itemCount * (itemHeight + this->_gridView->getSpacingHeight()), this->_gridView->getWidth(), itemHeight);
					this->setAnchors(true, true, true, false);
					this->_gridView->_updateRow(itemCount);
					this->_gridView->_updateScrollArea();
				}
				else
				{
					hlog::errorf(aprilui::logTag, "GridViewRow '%s' cannot be reattached to ScrollArea of GridView '%s', ScrollArea does not exist!", this->name.c_str(), this->parent->getName().c_str());
				}
			}
			else if (this->parent != NULL && dynamic_cast<ScrollArea*>(this->parent) == NULL)
			{
				this->_gridView = NULL;
				hlog::errorf(aprilui::logTag, "GridViewRow '%s' not attached to object of class GridView!", this->name.c_str());
			}
		}
	}

}

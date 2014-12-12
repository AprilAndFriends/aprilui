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
#include "ObjectGridViewRowTemplate.h"
#include "ObjectScrollArea.h"

namespace aprilui
{
	GridViewRowTemplate::GridViewRowTemplate(chstr name) : GridViewRow(name)
	{
		this->visible = false;
		this->enabled = false;
	}

	GridViewRowTemplate::GridViewRowTemplate(const GridViewRowTemplate& other) : GridViewRow(other)
	{
		this->visible = false;
		this->enabled = false;
	}

	GridViewRowTemplate::~GridViewRowTemplate()
	{
	}

	Object* GridViewRowTemplate::createInstance(chstr name)
	{
		return new GridViewRowTemplate(name);
	}

	void GridViewRowTemplate::update(float timeDelta)
	{
		// this object and its children do not update
	}

	void GridViewRowTemplate::_draw()
	{
		// this object and its children are not drawn
	}

	void GridViewRowTemplate::notifyEvent(chstr type, EventArgs* args)
	{
		Container::notifyEvent(type, args); // overrides GridViewRow's default events
		if (type == Event::AttachedToObject)
		{
			GridView* gridView = dynamic_cast<GridView*>(this->parent);
			if (gridView != NULL)
			{
				this->_gridView = gridView;
				this->_gridView->rowTemplate = this;
			}
			else if (this->parent != NULL && dynamic_cast<ScrollArea*>(this->parent) == NULL)
			{
				this->_gridView = NULL;
				hlog::errorf(aprilui::logTag, "GridViewRowTemplate '%s' not attached to object of class GridView!", this->name.c_str());
			}
		}
	}

}

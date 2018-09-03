/// @file
/// @version 5.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/aprilUtil.h>
#include <hltypes/hlog.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "Dataset.h"
#include "ObjectGridView.h"
#include "ObjectGridViewRowTemplate.h"
#include "ObjectScrollArea.h"

namespace aprilui
{
	GridViewRowTemplate::GridViewRowTemplate(chstr name) :
		GridViewRow(name)
	{
		this->visible = false;
		this->enabled = false;
	}

	GridViewRowTemplate::GridViewRowTemplate(const GridViewRowTemplate& other) :
		GridViewRow(other)
	{
		this->visible = false;
		this->enabled = false;
	}

	Object* GridViewRowTemplate::createInstance(chstr name)
	{
		return new GridViewRowTemplate(name);
	}

	void GridViewRowTemplate::_update(float timeDelta)
	{
		// this object and its children do not update
	}

	void GridViewRowTemplate::_draw()
	{
		// this object and its children are not drawn
	}

	GridViewRow* GridViewRowTemplate::_createRow(chstr name)
	{
		GridViewRow* row = GridViewRow::clone(); // clones only the superclass which is GridViewRow
		row->setName(name);
		this->gridView->addChild(row);
		row->setEnabled(true);
		row->setVisible(true);
		row->_cloneChildren(this->childrenObjects, this->childrenAnimators);
		harray<BaseObject*> objects = row->getDescendants();
		foreach (BaseObject*, it, objects)
		{
			(*it)->setName(april::generateName(name + "_"));
		}
		if (this->dataset != NULL)
		{
			this->dataset->registerObjects(row, false);
		}
		return row;
	}

	void GridViewRowTemplate::notifyEvent(chstr type, EventArgs* args)
	{
		Container::notifyEvent(type, args); // overrides GridViewRow's default events
		if (type == Event::AttachedToObject)
		{
			GridView* gridView = dynamic_cast<GridView*>(this->parent);
			if (gridView != NULL)
			{
				this->gridView = gridView;
				this->gridView->rowTemplate = this;
			}
			else if (this->parent != NULL && dynamic_cast<ScrollArea*>(this->parent) == NULL)
			{
				this->gridView = NULL;
				hlog::errorf(logTag, "GridViewRowTemplate '%s' not attached to object of class GridView!", this->name.cStr());
			}
		}
	}

}

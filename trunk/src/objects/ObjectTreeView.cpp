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
#include "ObjectTreeView.h"
#include "ObjectTreeViewNode.h"

namespace aprilui
{
	harray<PropertyDescription> TreeView::_propertyDescriptions;

	TreeView::TreeView(chstr name) : Container(name)
	{
		this->rootNode = NULL;
	}

	TreeView::~TreeView()
	{
	}

	Object* TreeView::createInstance(chstr name)
	{
		return new TreeView(name);
	}

	harray<PropertyDescription> TreeView::getPropertyDescriptions()
	{
		if (TreeView::_propertyDescriptions.size() == 0)
		{
			//TreeView::_propertyDescriptions += PropertyDescription("item_height", PropertyDescription::FLOAT);
		}
		return (Container::getPropertyDescriptions() + TreeView::_propertyDescriptions);
	}

}

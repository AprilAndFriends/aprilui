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
#include "ObjectTreeViewNode.h"

namespace aprilui
{
	TreeViewNode::TreeViewNode(chstr name) : Container(name)
	{
		this->_treeView = NULL;
	}

	TreeViewNode::~TreeViewNode()
	{
	}

	Object* TreeViewNode::createInstance(chstr name)
	{
		return new TreeViewNode(name);
	}

}

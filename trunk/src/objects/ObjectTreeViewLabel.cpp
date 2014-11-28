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
#include "ObjectTreeView.h"
#include "ObjectTreeViewLabel.h"
#include "ObjectTreeViewNode.h"

namespace aprilui
{
	TreeViewLabel::TreeViewLabel(chstr name) : Label(name)
	{
		this->_treeView = NULL;
		this->_treeViewNode = NULL;
	}

	TreeViewLabel::TreeViewLabel(const TreeViewLabel& other) : Label(other)
	{
		this->_treeView = NULL;
		this->_treeViewNode = NULL;
	}

	TreeViewLabel::~TreeViewLabel()
	{
	}

	Object* TreeViewLabel::createInstance(chstr name)
	{
		return new TreeViewLabel(name);
	}

	void TreeViewLabel::notifyEvent(chstr type, EventArgs* args)
	{
		Label::notifyEvent(type, args);
		if (type == Event::AttachedToObject)
		{
			TreeViewNode* treeViewNode = dynamic_cast<TreeViewNode*>(this->parent);
			if (treeViewNode != NULL)
			{
				if (this->_treeViewNode == NULL || treeViewNode->label == NULL)
				{
					this->_treeViewNode = treeViewNode;
					this->_treeViewNode->label = this;
					this->_treeView = this->_treeViewNode->_treeView;
					float width = this->_treeView->getExpanderWidth() + this->_treeView->getImageWidth() + this->_treeView->getSpacingWidth() * 2;
					this->setSize(this->_treeView->getWidth() - width, this->_treeView->getItemHeight());
					this->setAnchors(true, true, true, false);
					this->_treeView->_updateDisplay();
				}
				else
				{
					this->_treeView = NULL;
					this->_treeViewNode = NULL;
					hlog::errorf(aprilui::logTag, "TreeViewLabel '%s' cannot be assigned to TreeViewNode '%s', it already exist!", this->name.c_str(), this->parent->getName().c_str());
				}
			}
			else if (this->parent != NULL)
			{
				this->_treeView = NULL;
				this->_treeViewNode = NULL;
				hlog::errorf(aprilui::logTag, "TreeViewLabel '%s' not attached to object of class TreeViewNode!", this->name.c_str());
			}
		}
	}

}

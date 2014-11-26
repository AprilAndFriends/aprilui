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
#include "EventArgs.h"
#include "MemberCallbackEvent.h"
#include "ObjectTreeView.h"
#include "ObjectTreeViewExpander.h"
#include "ObjectTreeViewNode.h"

namespace aprilui
{
	TreeViewExpander::TreeViewExpander(chstr name) : ToggleButton(name)
	{
		this->_treeView = NULL;
		this->_treeViewNode = NULL;
	}

	TreeViewExpander::~TreeViewExpander()
	{
	}

	Object* TreeViewExpander::createInstance(chstr name)
	{
		return new TreeViewExpander(name);
	}

	void TreeViewExpander::notifyEvent(chstr type, EventArgs* args)
	{
		ToggleButton::notifyEvent(type, args);
		if (type == Event::AttachedToObject)
		{
			TreeViewNode* treeViewNode = dynamic_cast<TreeViewNode*>(this->parent);
			if (treeViewNode != NULL)
			{
				if (this->_treeViewNode == NULL && treeViewNode->_treeViewExpander == NULL)
				{
					this->_treeViewNode = treeViewNode;
					this->_treeViewNode->_treeViewExpander = this;
					this->_treeView = this->_treeViewNode->_treeView;
					this->setRect(0.0f, 0.0f, this->_treeView->getExpanderWidth(), this->_treeView->getNodeHeight());
					this->setAnchors(true, true, true, false);
					this->registerEvent(Event::Click, new MemberCallbackEvent<TreeViewExpander>(&TreeViewExpander::_onClick, this));
					this->_treeView->_updateDisplay();
				}
				else
				{
					this->_treeView = NULL;
					this->_treeViewNode = NULL;
					hlog::errorf(aprilui::logTag, "TreeViewExpander '%s' cannot be assigned to TreeViewNode '%s', it already exist!", this->name.c_str(), this->parent->getName().c_str());
				}
			}
			else if (this->parent != NULL)
			{
				this->_treeView = NULL;
				this->_treeViewNode = NULL;
				hlog::errorf(aprilui::logTag, "TreeViewExpander '%s' not attached to object of class TreeViewNode!", this->name.c_str());
			}
		}
	}

	void TreeViewExpander::_onClick(EventArgs* args)
	{
		if (this->_treeView != NULL)
		{
			this->_treeView->_updateDisplay();
		}
	}

}

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
#include "ObjectTreeViewImage.h"
#include "ObjectTreeViewNode.h"

namespace aprilui
{
	TreeViewImage::TreeViewImage(chstr name) : ImageBox(name)
	{
		this->_treeView = NULL;
		this->_treeViewNode = NULL;
	}

	TreeViewImage::~TreeViewImage()
	{
	}

	Object* TreeViewImage::createInstance(chstr name)
	{
		return new TreeViewImage(name);
	}

	void TreeViewImage::notifyEvent(chstr type, EventArgs* args)
	{
		ImageBox::notifyEvent(type, args);
		if (type == Event::AttachedToObject)
		{
			TreeViewNode* treeViewNode = dynamic_cast<TreeViewNode*>(this->parent);
			if (treeViewNode != NULL)
			{
				if (this->_treeViewNode == NULL && treeViewNode->_treeViewImage == NULL)
				{
					this->_treeViewNode = treeViewNode;
					this->_treeViewNode->_treeViewImage = this;
					this->_treeView = this->_treeViewNode->_treeView;
					this->setRect(this->_treeView->getExpanderWidth() + this->_treeView->getSpacingWidth(), 0.0f,
						this->_treeView->getImageWidth(), this->_treeView->getNodeHeight());
					this->setAnchors(true, true, true, false);
					this->_treeView->_updateDisplay();
				}
				else
				{
					this->_treeView = NULL;
					this->_treeViewNode = NULL;
					hlog::errorf(aprilui::logTag, "TreeViewImage '%s' cannot be assigned to TreeViewNode '%s', it already exist!", this->name.c_str(), this->parent->getName().c_str());
				}
			}
			else if (this->parent != NULL)
			{
				this->_treeView = NULL;
				this->_treeViewNode = NULL;
				hlog::errorf(aprilui::logTag, "TreeViewImage '%s' not attached to object of class TreeViewNode!", this->name.c_str());
			}
		}
	}

}

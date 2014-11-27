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
#include "ObjectScrollArea.h"
#include "ObjectTreeView.h"
#include "ObjectTreeViewExpander.h"
#include "ObjectTreeViewImage.h"
#include "ObjectTreeViewLabel.h"
#include "ObjectTreeViewNode.h"

namespace aprilui
{
	TreeViewNode::TreeViewNode(chstr name) : Container(name)
	{
		this->_treeView = NULL;
		this->_treeViewExpander = NULL;
		this->_treeViewImage = NULL;
		this->_treeViewLabel = NULL;
	}

	TreeViewNode::~TreeViewNode()
	{
	}

	Object* TreeViewNode::createInstance(chstr name)
	{
		return new TreeViewNode(name);
	}

	int TreeViewNode::getDepth()
	{
		if (this->_treeViewParentNode == NULL)
		{
			return (this->_treeView != NULL ? 0 : -1); // -1 indicates a problem
		}
		return (this->_treeViewParentNode->getDepth() + 1);
	}

	bool TreeViewNode::isExpanded()
	{
		return (this->_treeViewNodes.size() > 0 && this->_treeViewExpander != NULL && this->_treeViewExpander->isDerivedVisible() && this->_treeViewExpander->isToggled());
	}

	void TreeViewNode::_draw()
	{
		Container::_draw();
		if (this->_treeView != NULL)
		{
			april::Color color = this->_treeView->getConnectorColor();
			if (color.a > 0 && this->isExpanded())
			{
				float nodeHeight = this->_treeView->getNodeHeight();
				float expanderWidth = this->_treeView->getExpanderWidth();
				float spacingHeight = this->_treeView->getSpacingHeight();
				grect drawRect = this->_getDrawRect();
				drawRect.x -= expanderWidth * 0.5f + 1.0f + this->_treeView->getSpacingWidth();
				drawRect.y += nodeHeight + spacingHeight;
				drawRect.setSize(2.0f, (nodeHeight + spacingHeight) * (this->_treeViewNodes.size() - 1) + nodeHeight * 0.5f + 1.0f);
				april::rendersys->drawFilledRect(drawRect, color);
				drawRect.x += 2.0f;
				drawRect.y += nodeHeight * 0.5f - 1.0f;
				drawRect.setSize(expanderWidth * 0.5f - 1.0f, 2.0f);
				for_iter (i, 0, this->_treeViewNodes.size())
				{
					april::rendersys->drawFilledRect(drawRect, color);
					drawRect.y += nodeHeight + spacingHeight;
				}
			}
		}
	}

	int TreeViewNode::_updateDisplay(int offsetIndex)
	{
		int offset = 1;
		if (this->_treeView != NULL)
		{
			this->setX(this->_treeView->getExpanderWidth() + this->_treeView->getSpacingWidth());
			this->setY(offsetIndex * (this->_treeView->getNodeHeight() + this->_treeView->getSpacingHeight()));
			bool expanded = this->isExpanded();
			if (expanded)
			{
				foreach (TreeViewNode*, it, this->_treeViewNodes)
				{
					(*it)->setVisible(true);
					offset += (*it)->_updateDisplay(offset);
				}
			}
			else
			{
				foreach (TreeViewNode*, it, this->_treeViewNodes)
				{
					(*it)->setVisible(false);
				}
			}
			if (this->_treeViewExpander != NULL)
			{
				this->_treeViewExpander->setX(-this->_treeView->getExpanderWidth() - this->_treeView->getSpacingWidth());
				this->_treeViewExpander->setVisible(this->_treeViewNodes.size() > 0);
			}
			if (this->_treeViewImage != NULL)
			{
				this->_treeViewImage->setX(0.0f);
			}
			if (this->_treeViewLabel != NULL)
			{
				this->_treeViewLabel->setX(this->_treeView->getImageWidth() + this->_treeView->getSpacingWidth());
			}
		}
		return offset;
	}

	void TreeViewNode::notifyEvent(chstr type, EventArgs* args)
	{
		Container::notifyEvent(type, args);
		if (type == Event::AttachedToObject)
		{
			bool found = false;
			TreeView* treeView = dynamic_cast<TreeView*>(this->parent);
			if (treeView != NULL)
			{
				this->_treeViewParentNode = NULL;
				this->_treeView = treeView;
				found = true;
			}
			else
			{
				TreeViewNode* treeViewNode = dynamic_cast<TreeViewNode*>(this->parent);
				if (treeViewNode != NULL)
				{
					this->_treeViewParentNode = treeViewNode;
					this->_treeView = this->_treeViewParentNode->_treeView;
					found = true;
				}
			}
			if (found)
			{
				ScrollArea* scrollArea = this->_treeView->_getScrollArea();
				if (scrollArea != NULL)
				{
					int nodeCount = this->_treeView->getNodeCount();
					float nodeHeight = this->_treeView->getNodeHeight();
					float offset = 0.0f;
					this->_treeView->nodes += this;
					if (this->_treeViewParentNode == NULL)
					{
						// reattach to ScrollArea
						this->_treeView->rootNodes += this;
						this->_treeView->removeChild(this);
						scrollArea->addChild(this);
					}
					else
					{
						this->_treeViewParentNode->_treeViewNodes += this;
						offset = this->_treeView->getExpanderWidth() + this->_treeView->getSpacingWidth();
					}
					scrollArea->setVisible(true);
					// setup all properties
					this->setSize(this->_treeView->getWidth(), nodeHeight);
					this->setAnchors(true, true, true, false);
					this->_treeView->_updateDisplay();
				}
				else
				{
					this->_treeView = NULL;
					this->_treeViewParentNode = NULL;
					this->_treeViewNodes.clear();
					hlog::errorf(aprilui::logTag, "TreeViewNode '%s' cannot be reattached to ScrollArea of TreeView '%s', ScrollArea does not exist!", this->name.c_str(), this->parent->getName().c_str());
				}
			}
			else if (this->parent != NULL && dynamic_cast<ScrollArea*>(this->parent) == NULL)
			{
				this->_treeView = NULL;
				this->_treeViewParentNode = NULL;
				this->_treeViewNodes.clear();
				hlog::errorf(aprilui::logTag, "TreeViewNode '%s' not attached to object of class TreeView or TreeViewNode!", this->name.c_str());
			}
		}
	}

}

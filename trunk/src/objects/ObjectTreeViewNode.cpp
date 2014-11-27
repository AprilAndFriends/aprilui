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
	TreeViewNode::TreeViewNode(chstr name) : Container(name), ButtonBase()
	{
		this->depth = -1;
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

	hstr TreeViewNode::getName()
	{
		return Container::getName();
	}

	int TreeViewNode::getFocusIndex()
	{
		return Container::getFocusIndex();
	}

	Object* TreeViewNode::getParent()
	{
		return Container::getParent();
	}

	Dataset* TreeViewNode::getDataset()
	{
		return Container::getDataset();
	}

	bool TreeViewNode::isCursorInside()
	{
		return Container::isCursorInside();
	}

	bool TreeViewNode::isExpanded()
	{
		return (this->_treeViewNodes.size() > 0 && this->_treeViewExpander != NULL && this->_treeViewExpander->isDerivedVisible() && this->_treeViewExpander->isToggled());
	}

	bool TreeViewNode::isSelected()
	{
		return (this->_treeView != NULL && is_between_ie(this->_treeView->selectedIndex, 0, this->_treeView->nodes.size()) &&
			this->_treeView->nodes[this->_treeView->selectedIndex] == this);
	}

	void TreeViewNode::update(float timeDelta)
	{
		ButtonBase::update(timeDelta);
		Container::update(timeDelta);
	}

	void TreeViewNode::_draw()
	{
		Container::_draw();
		if (this->_treeView != NULL)
		{
			april::Color color = this->_treeView->getConnectorColor();
			if (color.a > 0 && this->isExpanded())
			{
				float nodeHeight = this->_treeView->getItemHeight();
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
			this->setY(offsetIndex * (this->_treeView->getItemHeight() + this->_treeView->getSpacingHeight()));
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
				this->_treeViewImage->setHitTest(HIT_TEST_DISABLED);
			}
			if (this->_treeViewLabel != NULL)
			{
				this->_treeViewLabel->setX(this->_treeView->getImageWidth() + this->_treeView->getSpacingWidth());
				this->_treeViewLabel->setHitTest(HIT_TEST_DISABLED);
			}
		}
		return offset;
	}

	bool TreeViewNode::triggerEvent(chstr type, april::Key keyCode)
	{
		return Container::triggerEvent(type, keyCode);
	}

	bool TreeViewNode::triggerEvent(chstr type, april::Key keyCode, chstr string)
	{
		return Container::triggerEvent(type, keyCode, string);
	}

	bool TreeViewNode::triggerEvent(chstr type, april::Key keyCode, gvec2 position, chstr string, void* userData)
	{
		return Container::triggerEvent(type, keyCode, position, string, userData);
	}

	bool TreeViewNode::triggerEvent(chstr type, april::Button buttonCode, chstr string, void* userData)
	{
		return Container::triggerEvent(type, buttonCode, string, userData);
	}

	bool TreeViewNode::triggerEvent(chstr type, chstr string, void* userData)
	{
		return Container::triggerEvent(type, string, userData);
	}

	bool TreeViewNode::triggerEvent(chstr type, void* userData)
	{
		return Container::triggerEvent(type, userData);
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
					this->_treeView->nodes += this;
					if (this->_treeViewParentNode == NULL)
					{
						// reattach to ScrollArea
						this->_treeView->rootNodes += this;
						this->_treeView->removeChild(this);
						scrollArea->addChild(this);
						this->depth = 0;
					}
					else
					{
						this->_treeViewParentNode->_treeViewNodes += this;
						this->depth = this->_treeViewParentNode->depth + 1;
					}
					scrollArea->setVisible(true);
					// setup all properties
					this->setSize(this->_treeView->getWidth() - this->_treeView->getExpanderWidth() - this->_treeView->getSpacingWidth(), this->_treeView->getItemHeight());
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

	void TreeViewNode::_setSelected()
	{
		if (this->_treeView != NULL)
		{
			this->_treeView->setSelectedIndex(this->_treeView->nodes.index_of(this));
		}
	}

	bool TreeViewNode::_mouseDown(april::Key keyCode)
	{
		bool result = ButtonBase::_mouseDown(keyCode);
		if (result)
		{
			this->triggerEvent(Event::MouseDown, keyCode);
		}
		return (result || Container::_mouseDown(keyCode));
	}

	bool TreeViewNode::_mouseUp(april::Key keyCode)
	{
		bool result = ButtonBase::_mouseUp(keyCode);
		bool up = false;
		if (this->hovered)
		{
			up = this->triggerEvent(Event::MouseUp, keyCode);
		}
		if (result)
		{
			this->_setSelected();
			this->triggerEvent(Event::Click, keyCode);
		}
		return (result || up || Container::_mouseUp(keyCode));
	}

	bool TreeViewNode::_mouseMove()
	{
		return (ButtonBase::_mouseMove() || Container::_mouseMove());
	}

	bool TreeViewNode::_buttonDown(april::Button buttonCode)
	{
		bool result = ButtonBase::_buttonDown(buttonCode);
		if (result)
		{
			this->triggerEvent(Event::ButtonDown, buttonCode);
		}
		return (result || Container::_buttonDown(buttonCode));
	}

	bool TreeViewNode::_buttonUp(april::Button buttonCode)
	{
		if (Container::onButtonUp(buttonCode))
		{
			return true;
		}
		bool result = ButtonBase::_buttonUp(buttonCode);
		bool up = false;
		if (this->hovered)
		{
			up = this->triggerEvent(Event::ButtonUp, buttonCode);
		}
		if (result)
		{
			this->_setSelected();
			this->triggerEvent(Event::ButtonTrigger, buttonCode);
		}
		return (result || up || Container::_buttonUp(buttonCode));
	}

	void TreeViewNode::_mouseCancel(april::Key keyCode)
	{
		ButtonBase::_mouseCancel(keyCode);
		Container::_mouseCancel(keyCode);
	}

}

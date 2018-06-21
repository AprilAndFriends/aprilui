/// @file
/// @version 5.0
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
	hmap<hstr, PropertyDescription> TreeViewNode::_propertyDescriptions;

	TreeViewNode::TreeViewNode(chstr name) :
		Container(name),
		ButtonBase()
	{
		this->depth = -1;
		this->expander = NULL;
		this->image = NULL;
		this->label = NULL;
		this->_treeView = NULL;
		this->_treeViewParentNode = NULL;
	}

	TreeViewNode::TreeViewNode(const TreeViewNode& other) :
		Container(other),
		ButtonBase(other)
	{
		this->depth = -1;
		this->expander = NULL;
		this->image = NULL;
		this->label = NULL;
		this->_treeView = NULL;
		this->_treeViewParentNode = NULL;
	}

	TreeViewNode::~TreeViewNode()
	{
	}

	Object* TreeViewNode::createInstance(chstr name)
	{
		return new TreeViewNode(name);
	}

	hmap<hstr, PropertyDescription>& TreeViewNode::getPropertyDescriptions() const
	{
		if (TreeViewNode::_propertyDescriptions.size() == 0)
		{
			TreeViewNode::_propertyDescriptions = Container::getPropertyDescriptions() + ButtonBase::getPropertyDescriptions();
		}
		return TreeViewNode::_propertyDescriptions;
	}

	hstr TreeViewNode::getName() const
	{
		return Container::getName();
	}

	int TreeViewNode::getFocusIndex() const
	{
		return Container::getFocusIndex();
	}

	Object* TreeViewNode::getParent() const
	{
		return Container::getParent();
	}

	Dataset* TreeViewNode::getDataset() const
	{
		return Container::getDataset();
	}

	bool TreeViewNode::isCursorInside() const
	{
		return Container::isCursorInside();
	}

	bool TreeViewNode::isExpanded() const
	{
		return (this->nodes.size() > 0 && this->expander != NULL && this->expander->isDerivedVisible() && this->expander->isToggled());
	}

	bool TreeViewNode::isSelected() const
	{
		return (this->_treeView != NULL && hbetweenIE(this->_treeView->selectedIndex, 0, this->_treeView->items.size()) &&
			this->_treeView->items[this->_treeView->selectedIndex] == this);
	}

	void TreeViewNode::_update(float timeDelta)
	{
		ButtonBase::_update(timeDelta);
		Container::_update(timeDelta);
	}

	void TreeViewNode::_draw()
	{
		april::Color drawColor = this->_makeDrawColor();
		april::rendersys->setBlendMode(april::BlendMode::Alpha);
		april::rendersys->setColorMode(april::ColorMode::Multiply);
		if (this->_treeView != NULL)
		{
			april::Color color = this->_getCurrentBackgroundColor() * drawColor;
			if (color.a > 0)
			{
				april::rendersys->drawFilledRect(this->_makeDrawRect(), color);
			}
		}
		Container::_draw();
		if (this->_treeView != NULL)
		{
			april::Color color = this->_treeView->getConnectorColor() * drawColor;
			if (color.a > 0 && this->isExpanded())
			{
				float itemHeight = this->_treeView->getItemHeight();
				float expanderWidth = this->_treeView->getExpanderWidth();
				float spacingHeight = this->_treeView->getSpacingHeight();
				grectf drawRect = this->_makeDrawRect();
				grectf vertical(-expanderWidth * 0.5f - 1.0f - this->_treeView->getSpacingWidth(), itemHeight + spacingHeight, 2.0f, itemHeight * 0.5f + 1.0f);
				vertical += drawRect.getPosition();
				drawRect.set(vertical.x + 2.0f, vertical.y + itemHeight * 0.5f - 1.0f, expanderWidth * 0.5f - 1.0f, 2.0f);
				float offset = 0.0f;
				for_iter (i, 0, this->nodes.size())
				{
					april::rendersys->drawFilledRect(drawRect, color);
					offset = (itemHeight + spacingHeight) * this->nodes[i]->_calcOffset();
					drawRect.y += offset;
					if (i < this->nodes.size() - 1)
					{
						vertical.h += offset;
					}
				}
				april::rendersys->drawFilledRect(vertical, color);
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
				foreach (TreeViewNode*, it, this->nodes)
				{
					(*it)->setVisible(true);
					offset += (*it)->_updateDisplay(offset);
				}
			}
			else
			{
				foreach (TreeViewNode*, it, this->nodes)
				{
					(*it)->setVisible(false);
				}
			}
			if (this->expander != NULL)
			{
				this->expander->setX(-this->_treeView->getExpanderWidth() - this->_treeView->getSpacingWidth());
				this->expander->setVisible(this->nodes.size() > 0);
			}
			if (this->image != NULL)
			{
				this->image->setX(0.0f);
				this->image->setHitTest(HitTest::Disabled);
			}
			if (this->label != NULL)
			{
				this->label->setX(this->_treeView->getImageWidth() + this->_treeView->getSpacingWidth());
				this->label->setHitTest(HitTest::Disabled);
			}
		}
		return offset;
	}

	int TreeViewNode::_calcOffset()
	{
		int offset = 1;
		if (this->isExpanded())
		{
			foreach (TreeViewNode*, it, this->nodes)
			{
				offset += (*it)->_calcOffset();
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
				if (this->_treeView->scrollArea != NULL)
				{
					this->_treeView->items += this;
					if (this->_treeViewParentNode == NULL)
					{
						// reattach to ScrollArea
						this->_treeView->nodes += this;
						this->_treeView->removeChild(this);
						this->_treeView->scrollArea->addChild(this);
						this->depth = 0;
					}
					else
					{
						this->_treeViewParentNode->nodes += this;
						this->depth = this->_treeViewParentNode->depth + 1;
					}
					// setup all properties
					this->setSize(this->_treeView->getWidth() - this->_treeView->getExpanderWidth() - this->_treeView->getSpacingWidth(), this->_treeView->getItemHeight());
					this->setAnchors(true, true, true, false);
					this->_treeView->_updateDisplay();
				}
				else
				{
					this->_treeView = NULL;
					this->_treeViewParentNode = NULL;
					this->nodes.clear();
					hlog::errorf(logTag, "TreeViewNode '%s' cannot be reattached to ScrollArea of TreeView '%s', ScrollArea does not exist!", this->name.cStr(), this->parent->getName().cStr());
				}
			}
			else if (this->parent != NULL && dynamic_cast<ScrollArea*>(this->parent) == NULL)
			{
				this->_treeView = NULL;
				this->_treeViewParentNode = NULL;
				this->nodes.clear();
				hlog::errorf(logTag, "TreeViewNode '%s' not attached to object of class TreeView or TreeViewNode!", this->name.cStr());
			}
		}
	}

	bool TreeViewNode::triggerEvent(chstr type, april::Key keyCode)
	{
		return Container::triggerEvent(type, keyCode);
	}

	bool TreeViewNode::triggerEvent(chstr type, april::Key keyCode, chstr string)
	{
		return Container::triggerEvent(type, keyCode, string);
	}

	bool TreeViewNode::triggerEvent(chstr type, april::Key keyCode, cgvec2f position, chstr string, void* userData)
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

	april::Color TreeViewNode::_getCurrentBackgroundColor() const
	{
		if (this->_treeView != NULL)
		{
			if (this->pushed)
			{
				return (!this->isSelected() ? this->_treeView->getPushedColor() : this->_treeView->getSelectedPushedColor());
			}
			if (this->hovered)
			{
				return (!this->isSelected() ? this->_treeView->getHoverColor() : this->_treeView->getSelectedHoverColor());
			}
			if (this->isSelected())
			{
				return this->_treeView->getSelectedColor();
			}
		}
		return april::Color::Clear;
	}

	void TreeViewNode::_setSelected()
	{
		if (this->_treeView != NULL)
		{
			this->_treeView->setSelectedIndex(this->_treeView->items.indexOf(this));
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

	void TreeViewNode::_mouseCancel(april::Key keyCode)
	{
		ButtonBase::_mouseCancel(keyCode);
		Container::_mouseCancel(keyCode);
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
		if (Container::_buttonUp(buttonCode))
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

}

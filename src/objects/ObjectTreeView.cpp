/// @file
/// @version 5.0
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
#include "apriluiUtil.h"
#include "Dataset.h"
#include "MemberCallbackEvent.h"
#include "ObjectScrollArea.h"
#include "ObjectTreeView.h"
#include "ObjectTreeViewExpander.h"
#include "ObjectTreeViewImage.h"
#include "ObjectTreeViewLabel.h"
#include "ObjectTreeViewNode.h"

namespace aprilui
{
	hmap<hstr, PropertyDescription> TreeView::_propertyDescriptions;
	hmap<hstr, PropertyDescription::Accessor*> TreeView::_getters;
	hmap<hstr, PropertyDescription::Accessor*> TreeView::_setters;

	TreeView::TreeView(chstr name) : SelectionContainer(name)
	{
		this->expanderWidth = 32.0f;
		this->imageWidth = 32.0f;
		this->spacingWidth = 0.0f;
		this->spacingHeight = 4.0f;
		this->connectorColor.set(april::Color::White, 192);
	}

	TreeView::TreeView(const TreeView& other) : SelectionContainer(other)
	{
		this->expanderWidth = other.expanderWidth;
		this->imageWidth = other.imageWidth;
		this->spacingWidth = other.spacingWidth;
		this->spacingHeight = other.spacingHeight;
		this->connectorColor = other.connectorColor;
	}

	TreeView::~TreeView()
	{
	}

	Object* TreeView::createInstance(chstr name)
	{
		return new TreeView(name);
	}

	hmap<hstr, PropertyDescription>& TreeView::getPropertyDescriptions() const
	{
		if (TreeView::_propertyDescriptions.size() == 0)
		{
			TreeView::_propertyDescriptions = SelectionContainer::getPropertyDescriptions();
			TreeView::_propertyDescriptions["expander_width"] = PropertyDescription("expander_width", PropertyDescription::Type::Float);
			TreeView::_propertyDescriptions["image_width"] = PropertyDescription("image_width", PropertyDescription::Type::Float);
			TreeView::_propertyDescriptions["spacing_width"] = PropertyDescription("spacing_width", PropertyDescription::Type::Float);
			TreeView::_propertyDescriptions["spacing_height"] = PropertyDescription("spacing_height", PropertyDescription::Type::Float);
			TreeView::_propertyDescriptions["connector_color"] = PropertyDescription("connector_color", PropertyDescription::Type::Color);
		}
		return TreeView::_propertyDescriptions;
	}


	hmap<hstr, PropertyDescription::Accessor*>& TreeView::_getGetters() const
	{
		if (TreeView::_getters.size() == 0)
		{
			TreeView::_getters = SelectionContainer::_getGetters();
			TreeView::_getters["expander_width"] = new PropertyDescription::Get<TreeView, float>(&TreeView::getExpanderWidth);
			TreeView::_getters["image_width"] = new PropertyDescription::Get<TreeView, float>(&TreeView::getImageWidth);
			TreeView::_getters["spacing_width"] = new PropertyDescription::Get<TreeView, float>(&TreeView::getSpacingWidth);
			TreeView::_getters["spacing_height"] = new PropertyDescription::Get<TreeView, float>(&TreeView::getSpacingHeight);
			TreeView::_getters["connector_color"] = new PropertyDescription::GetColor<TreeView>(&TreeView::getConnectorColor);
		}
		return TreeView::_getters;
	}

	hmap<hstr, PropertyDescription::Accessor*>& TreeView::_getSetters() const
	{
		if (TreeView::_setters.size() == 0)
		{
			TreeView::_setters = SelectionContainer::_getSetters();
			TreeView::_setters["expander_width"] = new PropertyDescription::Set<TreeView, float>(&TreeView::setExpanderWidth);
			TreeView::_setters["image_width"] = new PropertyDescription::Set<TreeView, float>(&TreeView::setImageWidth);
			TreeView::_setters["spacing_width"] = new PropertyDescription::Set<TreeView, float>(&TreeView::setSpacingWidth);
			TreeView::_setters["spacing_height"] = new PropertyDescription::Set<TreeView, float>(&TreeView::setSpacingHeight);
			TreeView::_setters["connector_color"] = new PropertyDescription::Set<TreeView, hstr>(&TreeView::setConnectorSymbolicColor);
		}
		return TreeView::_setters;
	}

	void TreeView::setExpanderWidth(const float& value)
	{
		if (this->expanderWidth != value)
		{
			this->expanderWidth = value;
			this->_updateDisplay();
		}
	}

	void TreeView::setImageWidth(const float& value)
	{
		if (this->imageWidth != value)
		{
			this->imageWidth = value;
			this->_updateDisplay();
		}
	}

	void TreeView::setSpacingWidth(const float& value)
	{
		if (this->spacingWidth != value)
		{
			this->spacingWidth = value;
			this->_updateDisplay();
		}
	}

	void TreeView::setSpacingHeight(const float& value)
	{
		if (this->spacingHeight != value)
		{
			this->spacingHeight = value;
			this->_updateDisplay();
		}
	}

	void TreeView::setConnectorColor(const april::Color& value)
	{
		if (this->connectorColor != value)
		{
			this->connectorColor = value;
			this->_updateDisplay();
		}
	}

	void TreeView::setConnectorSymbolicColor(chstr value)
	{
		this->setConnectorColor(aprilui::_makeColor(value));
	}

	TreeViewNode* TreeView::getSelected() const
	{
		return (hbetweenIE(this->selectedIndex, 0, this->items.size()) ? this->items[this->selectedIndex] : NULL);
	}

	void TreeView::setSelected(const harray<int>& nodeIndices)
	{
		if (nodeIndices.size() == 0)
		{
			hlog::errorf(logTag, "Cannot get node in TreeView '%s', no indices specified!", this->name.cStr());
			return;
		}
		TreeViewNode* node = NULL;
		if (!this->_findNode(nodeIndices, &node))
		{
			hlog::errorf(logTag, "Cannot select node with indices '%s' in TreeView '%s', one or more indices are out of bounds!", nodeIndices.cast<hstr>().joined(',').cStr(), this->name.cStr());
			return;
		}
		this->setSelectedIndex(this->items.indexOf(node));
	}

	int TreeView::getItemCount() const
	{
		return this->items.size();
	}

	bool TreeView::_findNode(harray<int> nodeIndices, TreeViewNode** node) const
	{
		*node = NULL;
		if (nodeIndices.size() == 0)
		{
			return true;
		}
		int index = nodeIndices.removeFirst();
		if (!hbetweenIE(index, 0, this->nodes.size()))
		{
			return false;
		}
		*node = this->nodes[index];
		while (nodeIndices.size() > 0)
		{
			index = nodeIndices.removeFirst();
			if (!hbetweenIE(index, 0, (*node)->nodes.size()))
			{
				*node = NULL;
				return false;
			}
			*node = (*node)->nodes[index];
		}
		return true;
	}

	TreeViewNode* TreeView::createItem(harray<int> nodeIndices, chstr name)
	{
		if (nodeIndices.size() == 0)
		{
			hlog::errorf(logTag, "Cannot create node in TreeView '%s', no indices specified!", this->name.cStr());
			return NULL;
		}
		if (this->scrollArea == NULL)
		{
			hlog::errorf(logTag, "Cannot create node with indices '%s' in TreeView '%s', no internal ScrollArea is present!", nodeIndices.cast<hstr>().joined(',').cStr(), this->name.cStr());
			return NULL;
		}
		int index = nodeIndices.removeLast();
		TreeViewNode* parent = NULL;
		if (!this->_findNode(nodeIndices, &parent))
		{
			hlog::errorf(logTag, "Cannot create node with indices '%s' in TreeView '%s', one or more indices are out of bounds!", nodeIndices.cast<hstr>().joined(',').cStr(), this->name.cStr());
			return NULL;
		}
		TreeViewNode* selected = this->getSelected();
		this->setSelectedIndex(-1);
		TreeViewNode* item = new TreeViewNode(name != "" ? name : april::generateName("aprilui::TreeViewNode"));
		if (parent == NULL)
		{
			this->registerChild(item);
			this->nodes -= item;
			this->nodes.insertAt(index, item);
		}
		else
		{
			parent->registerChild(item);
			parent->nodes -= item;
			parent->nodes.insertAt(index, item);
		}
		if (this->expanderWidth > 0.0f)
		{
			item->registerChild(new TreeViewExpander(april::generateName("aprilui::TreeViewExpander")));
		}
		if (this->imageWidth > 0.0f)
		{
			item->registerChild(new TreeViewImage(april::generateName("aprilui::TreeViewImage")));
		}
		item->registerChild(new TreeViewLabel(april::generateName("aprilui::TreeViewLabel")));
		if (selected != NULL)
		{
			this->setSelectedIndex(this->items.indexOf(selected));
		}
		this->_updateDisplay();
		return item;
	}

	bool TreeView::deleteItem(const harray<int>& nodeIndices)
	{
		if (nodeIndices.size() == 0)
		{
			hlog::errorf(logTag, "Cannot delete node in TreeView '%s', no indices specified!", this->name.cStr());
			return false;
		}
		TreeViewNode* node = NULL;
		if (!this->_findNode(nodeIndices, &node))
		{
			hlog::errorf(logTag, "Cannot delete node with indices '%s' in TreeView '%s', one or more indices are out of bounds!", nodeIndices.cast<hstr>().joined(',').cStr(), this->name.cStr());
			return false;
		}
		TreeViewNode* selected = this->getSelected();
		this->setSelectedIndex(-1);
		this->_deleteChildren(node);
		this->items -= node;
		int index = nodeIndices.last();
		if (selected != NULL && !this->items.has(selected)) // the currently selected object is one of the deleted objects, parent or previous should become selected
		{
			selected = NULL;
			if (nodeIndices.size() == 1)
			{
				if (this->nodes.size() > 1)
				{
					selected = this->nodes[hclamp(index - 1, 0, this->nodes.size() - 1)];
				}
			}
			else if (node->_treeViewParentNode->nodes.size() > 1)
			{
				selected = node->_treeViewParentNode->nodes[hclamp(index - 1, 0, node->_treeViewParentNode->nodes.size() - 1)];
			}
			else
			{
				selected = node->_treeViewParentNode;
			}
		}
		if (nodeIndices.size() == 1)
		{
			this->nodes.removeAt(nodeIndices[0]);
		}
		else
		{
			node->_treeViewParentNode->nodes -= node;
		}
		this->dataset->destroyObjects(node);
		if (selected != NULL)
		{
			this->setSelectedIndex(this->items.indexOf(selected));
		}
		this->_updateDisplay();
		return true;
	}

	void TreeView::_deleteChildren(TreeViewNode* node)
	{
		if (node->nodes.size() > 0)
		{
			foreach(TreeViewNode*, it, node->nodes)
			{
				this->_deleteChildren(*it);
			}
			this->items -= node->nodes;
			node->nodes.clear();
		}
	}

	TreeViewNode* TreeView::getItemAt(const harray<int>& nodeIndices) const
	{
		if (nodeIndices.size() == 0)
		{
			hlog::errorf(logTag, "Cannot get node in TreeView '%s', no indices specified!", this->name.cStr());
			return NULL;
		}
		TreeViewNode* node = NULL;
		this->_findNode(nodeIndices, &node);
		return node;
	}

	void TreeView::_updateDisplay()
	{
		int offset = 0;
		foreach(TreeViewNode*, it, this->nodes)
		{
			(*it)->setVisible(true);
			offset += (*it)->_updateDisplay(offset);
		}
		if (hbetweenIE(this->selectedIndex, 0, this->items.size()))
		{
			if (!this->items[this->selectedIndex]->isDerivedVisible())
			{
				TreeViewNode* node = this->items[this->selectedIndex]->_treeViewParentNode;
				while (node != NULL)
				{
					if (node->isDerivedVisible())
					{
						this->setSelectedIndex(this->items.indexOf(node));
						break;
					}
					node = node->_treeViewParentNode;
				}
			}
		}
		if (this->scrollArea != NULL)
		{
			float scrollOffsetY = this->scrollArea->getScrollOffsetY();
			this->scrollArea->setHeight(offset * this->itemHeight + (offset - 1) * this->spacingHeight);
			this->scrollArea->setScrollOffsetY(scrollOffsetY);
			this->scrollArea->setVisible(this->items.size() > 0);
			this->_optimizeVisibility();
		}
	}

	void TreeView::_updateItem(int index)
	{
	}

	void TreeView::_optimizeVisibility()
	{
		if (this->scrollArea != NULL)
		{
			grectf rect(this->scrollArea->getScrollOffset(), this->scrollArea->getParent()->getSize());
			foreach(TreeViewNode*, it, this->nodes)
			{
				if ((*it)->isExpanded() || rect.intersects((*it)->getRect()))
				{
					(*it)->setVisible(true);
					(*it)->setAwake(true);
				}
				else
				{
					(*it)->setVisible(false);
					(*it)->setAwake(false);
				}
			}
		}
	}

}

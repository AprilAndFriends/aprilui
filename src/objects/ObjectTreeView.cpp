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
#include "MemberCallbackEvent.h"
#include "ObjectScrollArea.h"
#include "ObjectTreeView.h"
#include "ObjectTreeViewExpander.h"
#include "ObjectTreeViewImage.h"
#include "ObjectTreeViewLabel.h"
#include "ObjectTreeViewNode.h"

namespace aprilui
{
	harray<PropertyDescription> TreeView::_propertyDescriptions;

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

	harray<PropertyDescription> TreeView::getPropertyDescriptions()
	{
		if (TreeView::_propertyDescriptions.size() == 0)
		{
			TreeView::_propertyDescriptions += PropertyDescription("expander_width", PropertyDescription::FLOAT);
			TreeView::_propertyDescriptions += PropertyDescription("image_width", PropertyDescription::FLOAT);
			TreeView::_propertyDescriptions += PropertyDescription("spacing_width", PropertyDescription::FLOAT);
			TreeView::_propertyDescriptions += PropertyDescription("spacing_height", PropertyDescription::FLOAT);
			TreeView::_propertyDescriptions += PropertyDescription("connector_color", PropertyDescription::HEXCOLOR);
		}
		return (SelectionContainer::getPropertyDescriptions() + TreeView::_propertyDescriptions);
	}

	void TreeView::setExpanderWidth(float value)
	{
		if (this->expanderWidth != value)
		{
			this->expanderWidth = value;
			this->_updateDisplay();
		}
	}

	void TreeView::setImageWidth(float value)
	{
		if (this->imageWidth != value)
		{
			this->imageWidth = value;
			this->_updateDisplay();
		}
	}

	void TreeView::setSpacingWidth(float value)
	{
		if (this->spacingWidth != value)
		{
			this->spacingWidth = value;
			this->_updateDisplay();
		}
	}

	void TreeView::setSpacingHeight(float value)
	{
		if (this->spacingHeight != value)
		{
			this->spacingHeight = value;
			this->_updateDisplay();
		}
	}

	void TreeView::setConnectorColor(april::Color value)
	{
		if (this->connectorColor != value)
		{
			this->connectorColor = value;
			this->_updateDisplay();
		}
	}

	TreeViewNode* TreeView::getSelected()
	{
		return (hbetweenIE(this->selectedIndex, 0, this->items.size()) ? this->items[this->selectedIndex] : NULL);
	}

	void TreeView::setSelected(harray<int> nodeIndices)
	{
		if (nodeIndices.size() == 0)
		{
			hlog::errorf(aprilui::logTag, "Cannot get node in TreeView '%s', no indices specified!", this->name.cStr());
			return;
		}
		TreeViewNode* node = NULL;
		if (!this->_findNode(nodeIndices, &node))
		{
			hlog::errorf(aprilui::logTag, "Cannot select node with indices '%s' in TreeView '%s', one or more indices are out of bounds!", nodeIndices.cast<hstr>().joined(',').cStr(), this->name.cStr());
			return;
		}
		this->setSelectedIndex(this->items.indexOf(node));
	}

	int TreeView::getItemCount()
	{
		return this->items.size();
	}

	bool TreeView::_findNode(harray<int> nodeIndices, TreeViewNode** node)
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
			hlog::errorf(aprilui::logTag, "Cannot create node in TreeView '%s', no indices specified!", this->name.cStr());
			return NULL;
		}
		if (this->scrollArea == NULL)
		{
			hlog::errorf(aprilui::logTag, "Cannot create node with indices '%s' in TreeView '%s', no internal ScrollArea is present!", nodeIndices.cast<hstr>().joined(',').cStr(), this->name.cStr());
			return NULL;
		}
		int index = nodeIndices.removeLast();
		TreeViewNode* parent = NULL;
		if (!this->_findNode(nodeIndices, &parent))
		{
			hlog::errorf(aprilui::logTag, "Cannot create node with indices '%s' in TreeView '%s', one or more indices are out of bounds!", nodeIndices.cast<hstr>().joined(',').cStr(), this->name.cStr());
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

	bool TreeView::deleteItem(harray<int> nodeIndices)
	{
		if (nodeIndices.size() == 0)
		{
			hlog::errorf(aprilui::logTag, "Cannot delete node in TreeView '%s', no indices specified!", this->name.cStr());
			return false;
		}
		TreeViewNode* node = NULL;
		if (!this->_findNode(nodeIndices, &node))
		{
			hlog::errorf(aprilui::logTag, "Cannot delete node with indices '%s' in TreeView '%s', one or more indices are out of bounds!", nodeIndices.cast<hstr>().joined(',').cStr(), this->name.cStr());
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
			foreach (TreeViewNode*, it, node->nodes)
			{
				this->_deleteChildren(*it);
			}
			this->items -= node->nodes;
			node->nodes.clear();
		}
	}

	TreeViewNode* TreeView::getItemAt(harray<int> nodeIndices)
	{
		if (nodeIndices.size() == 0)
		{
			hlog::errorf(aprilui::logTag, "Cannot get node in TreeView '%s', no indices specified!", this->name.cStr());
			return NULL;
		}
		TreeViewNode* node = NULL;
		this->_findNode(nodeIndices, &node);
		return node;
	}

	void TreeView::_updateDisplay()
	{
		int offset = 0;
		foreach (TreeViewNode*, it, this->nodes)
		{
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
		}
	}

	void TreeView::_updateItem(int index)
	{
	}

	hstr TreeView::getProperty(chstr name)
	{
		if (name == "expander_width")	return this->getExpanderWidth();
		if (name == "image_width")		return this->getImageWidth();
		if (name == "spacing_width")	return this->getSpacingWidth();
		if (name == "spacing_height")	return this->getSpacingHeight();
		if (name == "connector_color")	return this->getConnectorColor().hex();
		return SelectionContainer::getProperty(name);
	}

	bool TreeView::setProperty(chstr name, chstr value)
	{
		if		(name == "expander_width")	this->setExpanderWidth(value);
		else if (name == "image_width")		this->setImageWidth(value);
		else if (name == "spacing_width")	this->setSpacingWidth(value);
		else if (name == "spacing_height")	this->setSpacingHeight(value);
		else if (name == "connector_color")	this->setConnectorColor(value);
		else return SelectionContainer::setProperty(name, value);
		return true;
	}

}

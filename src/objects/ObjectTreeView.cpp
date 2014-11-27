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
#include "ObjectScrollArea.h"
#include "ObjectTreeView.h"
#include "ObjectTreeViewNode.h"

namespace aprilui
{
	harray<PropertyDescription> TreeView::_propertyDescriptions;

	TreeView::TreeView(chstr name) : Container(name)
	{
		this->nodeHeight = 32.0f;
		this->expanderWidth = 32.0f;
		this->imageWidth = 32.0f;
		this->spacingWidth = 0.0f;
		this->spacingHeight = 4.0f;
		this->connectorColor.set(april::Color::White, 192);
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
			TreeView::_propertyDescriptions += PropertyDescription("node_height", PropertyDescription::FLOAT);
			TreeView::_propertyDescriptions += PropertyDescription("expander_width", PropertyDescription::FLOAT);
			TreeView::_propertyDescriptions += PropertyDescription("image_width", PropertyDescription::FLOAT);
			TreeView::_propertyDescriptions += PropertyDescription("spacing_width", PropertyDescription::FLOAT);
			TreeView::_propertyDescriptions += PropertyDescription("spacing_height", PropertyDescription::FLOAT);
			TreeView::_propertyDescriptions += PropertyDescription("connector_color", PropertyDescription::HEXCOLOR);
		}
		return (Container::getPropertyDescriptions() + TreeView::_propertyDescriptions);
	}

	void TreeView::setNodeHeight(float value)
	{
		if (this->nodeHeight != value)
		{
			this->nodeHeight = value;
			this->_updateDisplay();
		}
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

	int TreeView::getNodeCount()
	{
		return this->nodes.size();
	}

	void TreeView::_updateDisplay()
	{
		int offset = 0;
		foreach (TreeViewNode*, it, this->rootNodes)
		{
			offset += (*it)->_updateDisplay(offset);
		}
		if (this->scrollArea != NULL)
		{
			float scrollOffsetY = this->scrollArea->getScrollOffsetY();
			this->scrollArea->setHeight(offset * this->nodeHeight + (offset - 1) * this->spacingHeight);
			this->scrollArea->setScrollOffsetY(scrollOffsetY);
		}
	}

	hstr TreeView::getProperty(chstr name)
	{
		if (name == "node_height")		return this->getNodeHeight();
		if (name == "expander_width")	return this->getExpanderWidth();
		if (name == "image_width")		return this->getImageWidth();
		if (name == "spacing_width")	return this->getSpacingWidth();
		if (name == "spacing_height")	return this->getSpacingHeight();
		if (name == "connector_color")	return this->getConnectorColor().hex();
		return Container::getProperty(name);
	}

	bool TreeView::setProperty(chstr name, chstr value)
	{
		if (name == "node_height")			this->setNodeHeight(value);
		else if (name == "expander_width")	this->setExpanderWidth(value);
		else if (name == "image_width")		this->setImageWidth(value);
		else if (name == "spacing_width")	this->setSpacingWidth(value);
		else if (name == "spacing_height")	this->setSpacingHeight(value);
		else if (name == "connector_color")	this->setConnectorColor(value);
		else return Container::setProperty(name, value);
		return true;
	}

	void TreeView::notifyEvent(chstr type, EventArgs* args)
	{
		Container::notifyEvent(type, args);
		if (type == Event::RegisteredInDataset)
		{
			if (this->scrollArea == NULL)
			{
				this->registerChild(new ScrollArea(april::generateName("aprilui::ScrollArea"))); // sets this->scrollArea
				this->scrollArea->setRect(this->rect);
				this->scrollArea->setAnchors(true, true, true, false);
				this->scrollArea->setVisible(false);
			}
		}
	}

}

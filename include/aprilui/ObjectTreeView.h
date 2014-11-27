/// @file
/// @version 4.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines a class for a tree view.

#ifndef APRILUI_TREE_VIEW_H
#define APRILUI_TREE_VIEW_H

#include <gtypes/Rectangle.h>
#include <hltypes/harray.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectContainer.h"

namespace aprilui
{
	class TreeViewExpander;
	class TreeViewImage;
	class TreeViewLabel;
	class TreeViewNode;

	class apriluiExport TreeView : public Container
	{
	public:
		friend class TreeViewExpander;
		friend class TreeViewImage;
		friend class TreeViewLabel;
		friend class TreeViewNode;

		TreeView(chstr name);
		~TreeView();
		inline hstr getClassName() const { return "TreeView"; }

		static Object* createInstance(chstr name);

		HL_DEFINE_GET(float, nodeHeight, NodeHeight);
		void setNodeHeight(float value);
		HL_DEFINE_GET(float, expanderWidth, ExpanderWidth);
		void setExpanderWidth(float value);
		HL_DEFINE_GET(float, imageWidth, ImageWidth);
		void setImageWidth(float value);
		HL_DEFINE_GET(float, spacingWidth, SpacingWidth);
		void setSpacingWidth(float value);
		HL_DEFINE_GET(float, spacingHeight, SpacingHeight);
		void setSpacingHeight(float value);
		HL_DEFINE_GET(april::Color, connectorColor, ConnectorColor);
		void setConnectorColor(april::Color value);
		int getNodeCount();

		harray<PropertyDescription> getPropertyDescriptions();

		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);

		void notifyEvent(chstr type, EventArgs* args);

	protected:
		float nodeHeight;
		float expanderWidth;
		float imageWidth;
		float spacingWidth;
		float spacingHeight;
		april::Color connectorColor;
		harray<TreeViewNode*> rootNodes;
		harray<TreeViewNode*> nodes;

		void _updateDisplay();

	private:
		static harray<PropertyDescription> _propertyDescriptions;

	};

}

#endif

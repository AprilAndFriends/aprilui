/// @file
/// @version 4.1
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
#include "ObjectSelectionContainer.h"

namespace aprilui
{
	class TreeViewExpander;
	class TreeViewImage;
	class TreeViewLabel;
	class TreeViewNode;

	class apriluiExport TreeView : public SelectionContainer
	{
		APRILUI_CLONEABLE(TreeView);
	public:
		friend class TreeViewExpander;
		friend class TreeViewImage;
		friend class TreeViewLabel;
		friend class TreeViewNode;

		TreeView(chstr name);
		~TreeView();
		inline hstr getClassName() const { return "TreeView"; }

		static Object* createInstance(chstr name);

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
		/// @note These are only the root TreeViewNodes in the TreeView.
		HL_DEFINE_GET(harray<TreeViewNode*>, nodes, Nodes);
		/// @note These are ALL the TreeViewNodes in the TreeView.
		/// @note These are not necessarily in the same order as they are organized within the TreeView.
		HL_DEFINE_GET(harray<TreeViewNode*>, items, Items);
		TreeViewNode* getSelected() const;
		void setSelected(harray<int> nodeIndices);
		int getItemCount() const;

		harray<PropertyDescription> getPropertyDescriptions();

		virtual TreeViewNode* createItem(harray<int> nodeIndices, chstr name = "");
		bool deleteItem(harray<int> nodeIndices);
		TreeViewNode* getItemAt(harray<int> nodeIndices) const;

		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);

	protected:
		float expanderWidth;
		float imageWidth;
		float spacingWidth;
		float spacingHeight;
		april::Color connectorColor;
		harray<TreeViewNode*> nodes;
		harray<TreeViewNode*> items;

		void _updateDisplay();
		void _updateItem(int index);
		void _optimizeVisibility();

		void _deleteChildren(TreeViewNode* node);
		bool _findNode(harray<int> nodeIndices, TreeViewNode** node) const;

	private:
		static harray<PropertyDescription> _propertyDescriptions;

	};

}

#endif

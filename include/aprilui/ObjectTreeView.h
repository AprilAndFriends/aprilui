/// @file
/// @version 5.1
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
		inline hstr getClassName() const { return "TreeView"; }
		static Object* createInstance(chstr name);

		hmap<hstr, PropertyDescription>& getPropertyDescriptions() const;

		HL_DEFINE_GET(float, expanderWidth, ExpanderWidth);
		void setExpanderWidth(const float& value);
		HL_DEFINE_GET(float, imageWidth, ImageWidth);
		void setImageWidth(const float& value);
		HL_DEFINE_GET(float, spacingWidth, SpacingWidth);
		void setSpacingWidth(const float& value);
		HL_DEFINE_GET(float, spacingHeight, SpacingHeight);
		void setSpacingHeight(const float& value);
		HL_DEFINE_GET(april::Color, connectorColor, ConnectorColor);
		void setConnectorColor(const april::Color& value);
		void setConnectorSymbolicColor(chstr value);
		/// @note These are only the root TreeViewNodes in the TreeView.
		HL_DEFINE_GET(harray<TreeViewNode*>, nodes, Nodes);
		/// @note These are ALL the TreeViewNodes in the TreeView.
		/// @note These are not necessarily in the same order as they are organized within the TreeView.
		HL_DEFINE_GET(harray<TreeViewNode*>, items, Items);
		TreeViewNode* getSelected() const;
		void setSelected(const harray<int>& nodeIndices);
		int getItemCount() const;

		virtual TreeViewNode* createItem(harray<int> nodeIndices, chstr name = "");
		bool deleteItem(const harray<int>& nodeIndices);
		TreeViewNode* getItemAt(const harray<int>& nodeIndices) const;

	protected:
		float expanderWidth;
		float imageWidth;
		float spacingWidth;
		float spacingHeight;
		april::Color connectorColor;
		harray<TreeViewNode*> nodes;
		harray<TreeViewNode*> items;

		hmap<hstr, PropertyDescription::Accessor*>& _getGetters() const;
		hmap<hstr, PropertyDescription::Accessor*>& _getSetters() const;

		void _updateDisplay();
		void _updateItem(int index);
		void _optimizeVisibility();

		void _deleteChildren(TreeViewNode* node);
		bool _findNode(harray<int> nodeIndices, TreeViewNode** node) const;

	private:
		static hmap<hstr, PropertyDescription> _propertyDescriptions;
		static hmap<hstr, PropertyDescription::Accessor*> _getters;
		static hmap<hstr, PropertyDescription::Accessor*> _setters;

	};

}
#endif

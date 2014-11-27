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
/// Defines a class for a tree view node.

#ifndef APRILUI_TREE_VIEW_NODE_H
#define APRILUI_TREE_VIEW_NODE_H

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectContainer.h"

namespace aprilui
{
	class TreeView;
	class TreeViewExpander;
	class TreeViewImage;
	class TreeViewLabel;

	class apriluiExport TreeViewNode : public Container
	{
	public:
		friend class TreeView;
		friend class TreeViewExpander;
		friend class TreeViewImage;
		friend class TreeViewLabel;

		TreeViewNode(chstr name);
		~TreeViewNode();
		inline hstr getClassName() const { return "TreeViewNode"; }

		/// @note A return value of -1 indicates a problem that there are no parent nodes and no TreeView to which this TreeViewNode was attached to.
		int getDepth();
		bool isExpanded();

		static Object* createInstance(chstr name);

		void notifyEvent(chstr type, EventArgs* args);

	protected:
		void _draw();

		int _updateDisplay(int offsetIndex);

	private:
		TreeView* _treeView;
		TreeViewNode* _treeViewParentNode;
		harray<TreeViewNode*> _treeViewNodes;
		TreeViewExpander* _treeViewExpander;
		TreeViewImage* _treeViewImage;
		TreeViewLabel* _treeViewLabel;

	};

}

#endif

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
#include "ObjectButtonBase.h"
#include "ObjectContainer.h"

namespace aprilui
{
	class TreeView;
	class TreeViewExpander;
	class TreeViewImage;
	class TreeViewLabel;

	class apriluiExport TreeViewNode : public Container, public ButtonBase
	{
	public:
		friend class TreeView;
		friend class TreeViewExpander;
		friend class TreeViewImage;
		friend class TreeViewLabel;

		TreeViewNode(chstr name);
		~TreeViewNode();
		inline hstr getClassName() const { return "TreeViewNode"; }

		static Object* createInstance(chstr name);

		hstr getName();
		int getFocusIndex();
		Object* getParent();
		Dataset* getDataset();
		bool isCursorInside();
		bool isSelected();

		/// @note A return value of -1 indicates a problem that there are no parent nodes and no TreeView to which this TreeViewNode was attached to.
		HL_DEFINE_GET(int, depth, Depth);
		bool isExpanded();

		void update(float timeDelta);

		bool triggerEvent(chstr type, april::Key keyCode);
		bool triggerEvent(chstr type, april::Key keyCode, chstr string);
		bool triggerEvent(chstr type, april::Key keyCode, gvec2 position, chstr string = "", void* userData = NULL);
		bool triggerEvent(chstr type, april::Button buttonCode, chstr string, void* userData = NULL);
		bool triggerEvent(chstr type, chstr string, void* userData = NULL);
		bool triggerEvent(chstr type, void* userData = NULL);

		void notifyEvent(chstr type, EventArgs* args);

	protected:
		int depth;

		void _draw();

		int _updateDisplay(int offsetIndex);
		void _setSelected();

		bool _mouseDown(april::Key keyCode);
		bool _mouseUp(april::Key keyCode);
		void _mouseCancel(april::Key keyCode);
		bool _mouseMove();
		bool _buttonDown(april::Button buttonCode);
		bool _buttonUp(april::Button buttonCode);

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

/// @file
/// @version 5.0
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
		APRILUI_CLONEABLE(TreeViewNode);
	public:
		friend class TreeView;
		friend class TreeViewExpander;
		friend class TreeViewImage;
		friend class TreeViewLabel;

		TreeViewNode(chstr name);
		~TreeViewNode();
		inline hstr getClassName() const { return "TreeViewNode"; }
		static Object* createInstance(chstr name);

		hmap<hstr, PropertyDescription>& getPropertyDescriptions() const;

		hstr getName() const;
		int getFocusIndex() const;
		Object* getParent() const;
		Dataset* getDataset() const;
		bool isCursorInside() const;

		HL_DEFINE_GET(int, depth, Depth);
		HL_DEFINE_GET(harray<TreeViewNode*>, nodes, Nodes);
		HL_DEFINE_GET(TreeViewExpander*, expander, Expander);
		HL_DEFINE_GET(TreeViewImage*, image, Image);
		HL_DEFINE_GET(TreeViewLabel*, label, Label);
		bool isExpanded() const;
		bool isSelected() const;

		void notifyEvent(chstr type, EventArgs* args);

		bool triggerEvent(chstr type, april::Key keyCode);
		bool triggerEvent(chstr type, april::Key keyCode, chstr string);
		bool triggerEvent(chstr type, april::Key keyCode, cgvec2f position, chstr string = "", void* userData = NULL);
		bool triggerEvent(chstr type, april::Button buttonCode, chstr string = "", void* userData = NULL);
		bool triggerEvent(chstr type, chstr string, void* userData = NULL);
		bool triggerEvent(chstr type, void* userData = NULL);

	protected:
		int depth;
		harray<TreeViewNode*> nodes;
		TreeViewExpander* expander;
		TreeViewImage* image;
		TreeViewLabel* label;

		void _update(float timeDelta);
		void _draw();

		int _updateDisplay(int offsetIndex);
		int _calcOffset();
		april::Color _getCurrentBackgroundColor() const;
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

		static hmap<hstr, PropertyDescription> _propertyDescriptions;

	};

}

#endif

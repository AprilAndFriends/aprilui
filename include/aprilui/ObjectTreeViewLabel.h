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
/// Defines a class for a tree view label.

#ifndef APRILUI_TREE_VIEW_LABEL_H
#define APRILUI_TREE_VIEW_LABEL_H

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectLabel.h"

namespace aprilui
{
	class TreeView;
	class TreeViewNode;

	class apriluiExport TreeViewLabel : public Label
	{
		APRILUI_CLONEABLE(TreeViewLabel);
	public:
		friend class TreeView;
		friend class TreeViewNode;

		TreeViewLabel(chstr name);
		inline hstr getClassName() const { return "TreeViewLabel"; }
		static Object* createInstance(chstr name);

		void notifyEvent(chstr type, EventArgs* args);

	private:
		TreeView* _treeView;
		TreeViewNode* _treeViewNode;

	};

}
#endif

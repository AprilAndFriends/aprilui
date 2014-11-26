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

	class apriluiExport TreeViewNode : public Container
	{
	public:
		friend class TreeView;

		TreeViewNode(chstr name);
		~TreeViewNode();
		inline hstr getClassName() const { return "TreeViewNode"; }

		static Object* createInstance(chstr name);

	private:
		TreeView* _treeView;

	};

}

#endif

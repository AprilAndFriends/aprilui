/// @file
/// @version 4.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines a class for a tree view expander button.

#ifndef APRILUI_TREE_VIEW_EXPANDER_H
#define APRILUI_TREE_VIEW_EXPANDER_H

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectToggleButton.h"

namespace aprilui
{
	class TreeView;
	class TreeViewNode;

	class apriluiExport TreeViewExpander : public ToggleButton
	{
		APRILUI_CLONEABLE(TreeViewExpander);
	public:
		friend class TreeView;
		friend class TreeViewNode;

		TreeViewExpander(chstr name);
		~TreeViewExpander();
		inline hstr getClassName() const { return "TreeViewExpander"; }

		static Object* createInstance(chstr name);

		void notifyEvent(chstr type, EventArgs* args);

	private:
		TreeView* _treeView;
		TreeViewNode* _treeViewNode;

		void _onClick(EventArgs* args);

	};

}

#endif

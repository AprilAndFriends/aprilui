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
	class TreeViewNode;

	class apriluiExport TreeView : public Container
	{
	public:
		friend class TreeViewNode;

		TreeView(chstr name);
		~TreeView();
		inline hstr getClassName() const { return "TreeView"; }

		static Object* createInstance(chstr name);

		harray<PropertyDescription> getPropertyDescriptions();

	protected:
		TreeViewNode* rootNode;
		harray<TreeViewNode*> nodes;

	private:
		static harray<PropertyDescription> _propertyDescriptions;

	};

}

#endif

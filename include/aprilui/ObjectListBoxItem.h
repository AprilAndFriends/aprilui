/// @file
/// @version 3.6
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines a list box item.

#ifndef APRILUI_LIST_BOX_ITEM_H
#define APRILUI_LIST_BOX_ITEM_H

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectLabel.h"

namespace aprilui
{
	class ListBox;

	class apriluiExport ListBoxItem : public Label
	{
	public:
		friend class ListBox;

		ListBoxItem(chstr name);
		~ListBoxItem();
		inline hstr getClassName() const { return "ListBoxItem"; }

		static Object* createInstance(chstr name);

	protected:
		void notifyEvent(chstr type, EventArgs* args);

	};
}

#endif

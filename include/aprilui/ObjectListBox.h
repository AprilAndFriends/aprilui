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
/// Defines a class for a list box.

#ifndef APRILUI_LIST_BOX_H
#define APRILUI_LIST_BOX_H

#include <gtypes/Rectangle.h>
#include <hltypes/harray.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectSelectionContainer.h"

namespace aprilui
{
	class ListBoxItem;

	class apriluiExport ListBox : public SelectionContainer
	{
		APRILUI_CLONEABLE(ListBox);
	public:
		friend class ListBoxItem;

		ListBox(chstr name);
		~ListBox();
		inline hstr getClassName() const { return "ListBox"; }

		static Object* createInstance(chstr name);

		HL_DEFINE_GET(april::Color, evenColor, EvenColor);
		void setEvenColor(april::Color value);
		HL_DEFINE_GET(april::Color, oddColor, OddColor);
		void setOddColor(april::Color value);
		HL_DEFINE_GET(harray<ListBoxItem*>, items, Items);
		ListBoxItem* getSelected();
		int getItemCount();

		harray<PropertyDescription> getPropertyDescriptions();

		virtual ListBoxItem* createItem(int index, chstr name = "");
		bool deleteItem(int index);
		ListBoxItem* getItemAt(int index);

		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);

	protected:
		april::Color evenColor;
		april::Color oddColor;
		harray<ListBoxItem*> items;

		void _updateDisplay();
		void _updateItem(int index);
		void _updateScrollArea();

	private:
		static harray<PropertyDescription> _propertyDescriptions;

	};
}

#endif

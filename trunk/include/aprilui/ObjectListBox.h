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
/// Defines a class for a list box.

#ifndef APRILUI_LIST_BOX_H
#define APRILUI_LIST_BOX_H

#include <gtypes/Rectangle.h>
#include <hltypes/harray.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectContainer.h"

namespace aprilui
{
	class ListBoxItem;

	class apriluiExport ListBox : public Container
	{
	public:
		friend class ListBoxItem;

		ListBox(chstr name);
		~ListBox();
		inline hstr getClassName() const { return "ListBox"; }

		static Object* createInstance(chstr name);

		HL_DEFINE_GET(float, itemHeight, ItemHeight);
		void setItemHeight(float value);
		HL_DEFINE_GET(int, selectedIndex, SelectedIndex);
		void setSelectedIndex(int value);
		HL_DEFINE_GET(april::Color, evenColor, EvenColor);
		void setEvenColor(april::Color value);
		HL_DEFINE_GET(april::Color, oddColor, OddColor);
		void setOddColor(april::Color value);
		HL_DEFINE_GET(april::Color, hoverColor, HoverColor);
		void setHoverColor(april::Color value);
		HL_DEFINE_GET(april::Color, pushedColor, PushedColor);
		void setPushedColor(april::Color value);
		HL_DEFINE_GET(april::Color, selectedColor, SelectedColor);
		void setSelectedColor(april::Color value);
		HL_DEFINE_GET(harray<ListBoxItem*>, items, Items);
		int getItemCount();

		harray<PropertyDescription> getPropertyDescriptions();

		virtual ListBoxItem* createItem(int index, chstr name = "");
		bool deleteItem(int index);
		ListBoxItem* getItemAt(int index);

		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);

		void notifyEvent(chstr type, EventArgs* args);

	protected:
		float itemHeight;
		april::Color evenColor;
		april::Color oddColor;
		april::Color hoverColor;
		april::Color pushedColor;
		april::Color selectedColor;
		int selectedIndex;
		harray<ListBoxItem*> items;

		void _updateSystem();
		void _updateItems();
		void _updateItem(int index);
		void _updateScrollArea();

	private:
		static harray<PropertyDescription> _propertyDescriptions;

	};
}

#endif

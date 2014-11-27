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
/// Defines a class for a list box.

#ifndef APRILUI_LIST_BOX_H
#define APRILUI_LIST_BOX_H

#include <gtypes/Rectangle.h>
#include <hltypes/harray.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectContainer.h"
#include "ObjectSelectionContainer.h"

namespace aprilui
{
	class ListBoxItem;

	class apriluiExport ListBox : public Container, public SelectionContainer
	{
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
		int getItemCount();

		harray<PropertyDescription> getPropertyDescriptions();

		virtual ListBoxItem* createItem(int index, chstr name = "");
		bool deleteItem(int index);
		ListBoxItem* getItemAt(int index);

		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);

		void notifyEvent(chstr type, EventArgs* args);

		bool triggerEvent(chstr type, april::Key keyCode);
		bool triggerEvent(chstr type, april::Key keyCode, chstr string);
		bool triggerEvent(chstr type, april::Key keyCode, gvec2 position, chstr string = "", void* userData = NULL);
		bool triggerEvent(chstr type, april::Button buttonCode, chstr string, void* userData = NULL);
		bool triggerEvent(chstr type, chstr string, void* userData = NULL);
		bool triggerEvent(chstr type, void* userData = NULL);

	protected:
		april::Color evenColor;
		april::Color oddColor;
		harray<ListBoxItem*> items;

		ScrollArea* _getInternalScrollArea();
		void _updateDisplay();
		void _updateItem(int index);
		void _updateScrollArea();

	private:
		static harray<PropertyDescription> _propertyDescriptions;

	};
}

#endif

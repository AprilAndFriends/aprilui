/// @file
/// @version 5.1
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
		inline hstr getClassName() const { return "ListBox"; }
		static Object* createInstance(chstr name);

		hmap<hstr, PropertyDescription>& getPropertyDescriptions() const;

		HL_DEFINE_GET(april::Color, evenColor, EvenColor);
		void setEvenColor(const april::Color& value);
		void setEvenSymbolicColor(chstr value);
		HL_DEFINE_GET(april::Color, oddColor, OddColor);
		void setOddColor(const april::Color& value);
		void setOddSymbolicColor(chstr value);
		HL_DEFINE_GET(harray<ListBoxItem*>, items, Items);
		ListBoxItem* getSelected() const;
		int getItemCount() const;

		virtual ListBoxItem* createItem(int index, chstr name = "");
		bool deleteItem(int index);
		ListBoxItem* getItemAt(int index);
		void snapToSelectedItem();

	protected:
		april::Color evenColor;
		april::Color oddColor;
		harray<ListBoxItem*> items;

		hmap<hstr, PropertyDescription::Accessor*>& _getGetters() const;
		hmap<hstr, PropertyDescription::Accessor*>& _getSetters() const;

		void _updateDisplay();
		void _updateItem(int index);
		void _updateScrollArea();
		void _optimizeVisibility();

	private:
		static hmap<hstr, PropertyDescription> _propertyDescriptions;
		static hmap<hstr, PropertyDescription::Accessor*> _getters;
		static hmap<hstr, PropertyDescription::Accessor*> _setters;

	};

}
#endif

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
/// Defines a base class for all objects that contain selectable children.

#ifndef APRILUI_SELECTION_BASE_H
#define APRILUI_SELECTION_BASE_H

#include <april/Keys.h>
#include <hltypes/harray.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectContainer.h"

namespace aprilui
{
	class apriluiExport SelectionContainer : public Container
	{
		APRILUI_CLONEABLE_ABSTRACT(SelectionContainer);
	public:
		SelectionContainer(chstr name);
		inline hstr getClassName() const { return "SelectionContainer"; }

		hmap<hstr, PropertyDescription>& getPropertyDescriptions() const;

		HL_DEFINE_GET(int, selectedIndex, SelectedIndex);
		void setSelectedIndex(const int& value);
		HL_DEFINE_GET(float, itemHeight, ItemHeight);
		void setItemHeight(const float& value);
		HL_DEFINE_GET(april::Color, pushedColor, PushedColor);
		void setPushedColor(const april::Color& value);
		void setPushedSymbolicColor(chstr value);
		HL_DEFINE_GET(april::Color, hoverColor, HoverColor);
		void setHoverColor(const april::Color& value);
		void setHoverSymbolicColor(chstr value);
		HL_DEFINE_GET(april::Color, selectedColor, SelectedColor);
		void setSelectedColor(const april::Color& value);
		void setSelectedSymbolicColor(chstr value);
		HL_DEFINE_GET(april::Color, selectedPushedColor, SelectedPushedColor);
		void setSelectedPushedColor(const april::Color& value);
		void setSelectedPushedSymbolicColor(chstr value);
		HL_DEFINE_GET(april::Color, selectedHoverColor, SelectedHoverColor);
		void setSelectedHoverColor(const april::Color& value);
		void setSelectedHoverSymbolicColor(chstr value);
		HL_DEFINE_IS(allowDrag, AllowDrag);
		void setAllowDrag(const bool& value);

		virtual int getItemCount() const = 0;

		void notifyEvent(chstr type, EventArgs* args);

	protected:
		int selectedIndex;
		float itemHeight;
		april::Color pushedColor;
		april::Color hoverColor;
		april::Color selectedColor;
		april::Color selectedPushedColor;
		april::Color selectedHoverColor;
		bool allowDrag;

		hmap<hstr, PropertyDescription::Accessor*>& _getGetters() const;
		hmap<hstr, PropertyDescription::Accessor*>& _getSetters() const;

		void _update(float timeDelta);

		virtual void _updateDisplay() = 0;
		virtual void _updateItem(int index) = 0;
		virtual void _optimizeVisibility();

	private:
		static hmap<hstr, PropertyDescription> _propertyDescriptions;
		static hmap<hstr, PropertyDescription::Accessor*> _getters;
		static hmap<hstr, PropertyDescription::Accessor*> _setters;

	};
	
}
#endif

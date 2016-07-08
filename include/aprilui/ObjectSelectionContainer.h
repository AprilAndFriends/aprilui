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
		virtual ~SelectionContainer();
		inline hstr getClassName() const { return "SelectionContainer"; }

		HL_DEFINE_GET(int, selectedIndex, SelectedIndex);
		void setSelectedIndex(int value);
		HL_DEFINE_GET(float, itemHeight, ItemHeight);
		void setItemHeight(float value);
		HL_DEFINE_GET(april::Color, hoverColor, HoverColor);
		void setHoverColor(april::Color value);
		HL_DEFINE_GET(april::Color, pushedColor, PushedColor);
		void setPushedColor(april::Color value);
		HL_DEFINE_GET(april::Color, selectedColor, SelectedColor);
		void setSelectedColor(april::Color value);
		HL_DEFINE_GET(april::Color, selectedHoverColor, SelectedHoverColor);
		void setSelectedHoverColor(april::Color value);
		HL_DEFINE_GET(april::Color, selectedPushedColor, SelectedPushedColor);
		void setSelectedPushedColor(april::Color value);
		HL_DEFINE_IS(allowDrag, AllowDrag);
		void setAllowDrag(bool value);

		virtual int getItemCount() const = 0;

		virtual harray<PropertyDescription> getPropertyDescriptions() const;

		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);

		void notifyEvent(chstr type, EventArgs* args);

	protected:
		int selectedIndex;
		float itemHeight;
		april::Color hoverColor;
		april::Color pushedColor;
		april::Color selectedColor;
		april::Color selectedHoverColor;
		april::Color selectedPushedColor;
		bool allowDrag;

		void _update(float timeDelta);

		virtual void _updateDisplay() = 0;
		virtual void _updateItem(int index) = 0;
		virtual void _optimizeVisibility();

	private:
		static harray<PropertyDescription> _propertyDescriptions;

	};
	
}

#endif

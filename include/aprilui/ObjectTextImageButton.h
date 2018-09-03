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
/// Defines an image button with text.

#ifndef APRILUI_TEXT_IMAGE_BUTTON_H
#define APRILUI_TEXT_IMAGE_BUTTON_H

#include <april/Keys.h>
#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectImageButton.h"
#include "ObjectLabelBase.h"

namespace aprilui
{
	class apriluiExport TextImageButton : public ImageButton, public LabelBase
	{
		APRILUI_CLONEABLE(TextImageButton);
	public:
		TextImageButton(chstr name);
		inline hstr getClassName() const { return "TextImageButton"; }
		static Object* createInstance(chstr name);

		hmap<hstr, PropertyDescription>& getPropertyDescriptions() const;

		HL_DEFINE_GET(april::Color, pushedTextColor, PushedTextColor);
		void setPushedTextColor(const april::Color& value);
		void setPushedTextSymbolicColor(chstr value);
		HL_DEFINE_GET(april::Color, hoverTextColor, HoverTextColor);
		void setHoverTextColor(const april::Color& value);
		void setHoverTextSymbolicColor(chstr value);
		HL_DEFINE_GET(april::Color, disabledTextColor, DisabledTextColor);
		void setDisabledTextColor(const april::Color& value);
		void setDisabledTextSymbolicColor(chstr value);
		Dataset* getDataset() const;
		hstr getAutoScaledFont();

		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);

		void notifyEvent(chstr type, EventArgs* args);

		bool triggerEvent(chstr type, april::Key keyCode);
		bool triggerEvent(chstr type, april::Key keyCode, chstr string);
		bool triggerEvent(chstr type, april::Key keyCode, cgvec2f position, chstr string = "", void* userData = NULL);
		bool triggerEvent(chstr type, april::Button buttonCode, chstr string = "", void* userData = NULL);
		bool triggerEvent(chstr type, chstr string, void* userData = NULL);
		bool triggerEvent(chstr type, void* userData = NULL);

	protected:
		april::Color pushedTextColor;
		april::Color hoverTextColor;
		april::Color disabledTextColor;
		
		hmap<hstr, PropertyDescription::Accessor*>& _getGetters() const;
		hmap<hstr, PropertyDescription::Accessor*>& _getSetters() const;

		void _draw();

	private:
		static hmap<hstr, PropertyDescription> _propertyDescriptions;
		static hmap<hstr, PropertyDescription::Accessor*> _getters;
		static hmap<hstr, PropertyDescription::Accessor*> _setters;

		bool _usePushedTextColor;
		bool _useHoverTextColor;
		bool _useDisabledTextColor;

	};
	
}
#endif

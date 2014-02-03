/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
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
	public:
		TextImageButton(chstr name, grect rect);
		~TextImageButton();
		static Object* createInstance(chstr name, grect rect);

		HL_DEFINE_GETSET(april::Color, hoverTextColor, HoverTextColor);
		HL_DEFINE_GETSET(april::Color, pushedTextColor, PushedTextColor);
		HL_DEFINE_GETSET(april::Color, disabledTextColor, DisabledTextColor);
		Dataset* getDataset();

		void notifyEvent(chstr name, void* params);
		// TODO - this needs to be seriously refactored
		bool triggerEvent(chstr name, april::Key keyCode = april::AK_NONE, chstr extra = "");
		// TODO - this needs to be seriously refactored
		bool triggerEvent(chstr name, april::Button buttonCode, chstr extra = "");
		// TODO - this needs to be seriously refactored
		bool triggerEvent(chstr name, float x, float y, april::Key keyCode = april::AK_NONE, chstr extra = "");
		hstr getProperty(chstr name, bool* propertyExists = NULL);
		bool setProperty(chstr name, chstr value);

	protected:
		april::Color hoverTextColor;
		april::Color pushedTextColor;
		april::Color disabledTextColor;
		
		void OnDraw();

	private:
		bool _useHoverTextColor;
		bool _usePushedTextColor;
		bool _useDisabledTextColor;

	};
	
}

#endif

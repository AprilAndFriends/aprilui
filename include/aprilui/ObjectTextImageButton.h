/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.75
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

		Dataset* getDataset();
		bool isUseBackground() { return mUseBackground; }
		void setUseBackground(bool value) { mUseBackground = value; }
		april::Color getHoverTextColor() { return mHoverTextColor; }
		void setHoverTextColor(april::Color value) { mHoverTextColor = value; _mUseHoverTextColor = true; }
		void setHoverTextColor(chstr value) { mHoverTextColor.set(value); _mUseHoverTextColor = true; }
		april::Color getPushedTextColor() { return mPushedTextColor; }
		void setPushedTextColor(april::Color value) { mPushedTextColor = value; _mUsePushedTextColor = true; }
		void setPushedTextColor(chstr value) { mPushedTextColor.set(value); _mUsePushedTextColor = true; }
		april::Color getDisabledTextColor() { return mDisabledTextColor; }
		void setDisabledTextColor(april::Color value) { mDisabledTextColor = value; _mUseDisabledTextColor = true; }
		void setDisabledTextColor(chstr value) { mDisabledTextColor.set(value); _mUseDisabledTextColor = true; }

		hstr getProperty(chstr name, bool* property_exists = NULL);
		void notifyEvent(chstr name, void* params);
		bool setProperty(chstr name, chstr value);

	protected:
		bool mUseBackground;
		april::Color mHoverTextColor;
		april::Color mPushedTextColor;
		april::Color mDisabledTextColor;
		
		void OnDraw();

	private:
		bool _mUseHoverTextColor;
		bool _mUsePushedTextColor;
		bool _mUseDisabledTextColor;

	};
	
}

#endif

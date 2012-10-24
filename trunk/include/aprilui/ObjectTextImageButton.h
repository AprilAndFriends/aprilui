/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.3
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
		april::Color getHoverTextColor() { return this->mHoverTextColor; }
		void setHoverTextColor(april::Color value) { this->mHoverTextColor = value; this->_mUseHoverTextColor = true; }
		void setHoverTextColor(chstr value) { this->mHoverTextColor.set(value); this->_mUseHoverTextColor = true; }
		april::Color getPushedTextColor() { return this->mPushedTextColor; }
		void setPushedTextColor(april::Color value) { this->mPushedTextColor = value; this->_mUsePushedTextColor = true; }
		void setPushedTextColor(chstr value) { this->mPushedTextColor.set(value); this->_mUsePushedTextColor = true; }
		april::Color getDisabledTextColor() { return this->mDisabledTextColor; }
		void setDisabledTextColor(april::Color value) { this->mDisabledTextColor = value; this->_mUseDisabledTextColor = true; }
		void setDisabledTextColor(chstr value) { this->mDisabledTextColor.set(value); this->_mUseDisabledTextColor = true; }

		void notifyEvent(chstr name, void* params);
		hstr getProperty(chstr name, bool* property_exists = NULL);
		bool setProperty(chstr name, chstr value);

	protected:
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

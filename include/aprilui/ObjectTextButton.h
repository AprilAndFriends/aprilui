/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.91
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Defines a button with text.

#ifndef APRILUI_TEXT_BUTTON_H
#define APRILUI_TEXT_BUTTON_H

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectButtonBase.h"
#include "ObjectLabel.h"

namespace aprilui
{
	class apriluiExport TextButton : public Label, public ButtonBase
	{
	public:
		TextButton(chstr name, grect rect);
		~TextButton();
		static Object* createInstance(chstr name, grect rect);
		
		hstr getName();
		bool isCursorInside();
		bool isUseBackground() { return this->mUseBackground; }
		void setUseBackground(bool value) { this->mUseBackground = value; }
		april::Color getHoverTextColor() { return this->mHoverTextColor; }
		void setHoverTextColor(april::Color value) { this->mHoverTextColor = value; }
		void setHoverTextColor(chstr value) { this->mHoverTextColor.set(value); }
		april::Color getPushedTextColor() { return this->mPushedTextColor; }
		void setPushedTextColor(april::Color value) { this->mPushedTextColor = value; }
		void setPushedTextColor(chstr value) { this->mPushedTextColor.set(value); }
		april::Color getDisabledTextColor() { return this->mDisabledTextColor; }
		void setDisabledTextColor(april::Color value) { this->mDisabledTextColor = value; }
		void setDisabledTextColor(chstr value) { this->mDisabledTextColor.set(value); }
		Object* getParent();
		Dataset* getDataset();

		hstr getProperty(chstr name, bool* property_exists = NULL);
		bool setProperty(chstr name, chstr value);
		
		bool onMouseDown(int button);
		bool onMouseUp(int button);
		void onMouseMove();
		void cancelMouseDown();
		
		DEPRECATED_ATTRIBUTE bool onMouseDown(float x, float y, int button) { return this->onMouseDown(button); }
		DEPRECATED_ATTRIBUTE bool onMouseUp(float x, float y, int button) { return this->onMouseUp(button); }
		DEPRECATED_ATTRIBUTE void onMouseMove(float x, float y) { this->onMouseMove(); }

	protected:
		bool mUseBackground;
		april::Color mHoverTextColor;
		april::Color mPushedTextColor;
		april::Color mDisabledTextColor;
		
		void update(float k);
		void OnDraw();
		
	};

}

#endif

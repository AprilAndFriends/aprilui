/// @file
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
/// Defines a button with 2 states.

#ifndef APRILUI_TOGGLE_BUTTON_H
#define APRILUI_TOGGLE_BUTTON_H

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectImageButton.h"

namespace aprilui
{
	class apriluiExport ToggleButton : public ImageButton
	{
	public:
		ToggleButton(chstr name, grect rect);
		~ToggleButton();
		static Object* createInstance(chstr name, grect rect);

		bool isToggled() { return this->mToggled; }
		void setToggled(bool value) { this->mToggled = value; }
		Image* getToggledNormalImage() { return this->mToggledNormalImage; }
		void setToggledNormalImage(Image* image) { this->mToggledNormalImage = image; }
		void setToggledNormalImageByName(chstr image);
		Image* getToggledHoverImage() { return this->mToggledHoverImage; }
		void setToggledHoverImage(Image* image) { this->mToggledHoverImage = image; }
		void setToggledHoverImageByName(chstr image);
		Image* getToggledPushedImage() { return this->mToggledPushedImage; }
		void setToggledPushedImage(Image* image) { this->mToggledPushedImage = image; }
		void setToggledPushedImageByName(chstr image);
		Image* getToggledDisabledImage() { return this->mToggledDisabledImage; }
		void setToggledDisabledImage(Image* image) { this->mToggledDisabledImage = image; }
		void setToggledDisabledImageByName(chstr image);

		void toggle() { mToggled = !mToggled; }
		void turnOn() { mToggled = true; }
		void turnOff() { mToggled = false; }
		
		hstr getProperty(chstr name, bool* property_exists = NULL);
		bool setProperty(chstr name, chstr value);

		bool onMouseUp(int button);

	protected:
		bool mToggled;
		Image* mToggledNormalImage;
		Image* mToggledHoverImage;
		Image* mToggledPushedImage;
		Image* mToggledDisabledImage;

		void update(float k);
		void OnDraw();
		
	};
}

#endif

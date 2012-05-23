/// @file
/// @author  Boris Mikic
/// @version 1.7
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

		bool isToggled() { return mToggled; }
		void setToggled(bool value) { mToggled = value; }
		Image* getToggledNormalImage() { return mToggledNormalImage; }
		void setToggledNormalImage(Image* image) { mToggledNormalImage = image; }
		void setToggledNormalImageByName(chstr image);
		Image* getToggledHoverImage() { return mToggledHoverImage; }
		void setToggledHoverImage(Image* image) { mToggledHoverImage = image; }
		void setToggledHoverImageByName(chstr image);
		Image* getToggledPushedImage() { return mToggledPushedImage; }
		void setToggledPushedImage(Image* image) { mToggledPushedImage = image; }
		void setToggledPushedImageByName(chstr image);
		Image* getToggledDisabledImage() { return mToggledDisabledImage; }
		void setToggledDisabledImage(Image* image) { mToggledDisabledImage = image; }
		void setToggledDisabledImageByName(chstr image);

		void toggle() { mToggled = !mToggled; }
		void turnOn() { mToggled = true; }
		void turnOff() { mToggled = false; }
		
		bool onMouseUp(int button);

		hstr getProperty(chstr name, bool* property_exists = NULL);
		bool setProperty(chstr name, chstr value);

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

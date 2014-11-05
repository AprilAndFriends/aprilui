/// @file
/// @version 3.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
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
		inline hstr getClassName() const { return "ToggleButton"; }

		static Object* createInstance(chstr name, grect rect);

		HL_DEFINE_ISSET(toggled, Toggled);
		HL_DEFINE_GET(Image*, toggledNormalImage, ToggledNormalImage);
		HL_DEFINE_GET(Image*, toggledHoverImage, ToggledHoverImage);
		HL_DEFINE_GET(Image*, toggledPushedImage, ToggledPushedImage);
		HL_DEFINE_GET(Image*, toggledDisabledImage, ToggledDisabledImage);
		HL_DEFINE_GET(hstr, toggledNormalImageName, ToggledNormalImageName);
		HL_DEFINE_GET(hstr, toggledHoverImageName, ToggledHoverImageName);
		HL_DEFINE_GET(hstr, toggledPushedImageName, ToggledPushedImageName);
		HL_DEFINE_GET(hstr, toggledDisabledImageName, ToggledDisabledImageName);
		void setToggledNormalImage(Image* image);
		void setToggledHoverImage(Image* image);
		void setToggledPushedImage(Image* image);
		void setToggledDisabledImage(Image* image);
		void setToggledNormalImageByName(chstr name);
		void setToggledHoverImageByName(chstr name);
		void setToggledPushedImageByName(chstr name);
		void setToggledDisabledImageByName(chstr name);

		harray<PropertyDescription> getPropertyDescriptions();

		/// @brief Optimized versions.
		bool trySetToggledNormalImageByName(chstr name);
		bool trySetToggledHoverImageByName(chstr name);
		bool trySetToggledPushedImageByName(chstr name);
		bool trySetToggledDisabledImageByName(chstr name);
		
		harray<Image*> getUsedImages();

		void update(float timeDelta);

		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);

		void toggle();
		virtual void turnOn();
		virtual void turnOff();
		
	protected:
		bool toggled;
		Image* toggledNormalImage;
		Image* toggledHoverImage;
		Image* toggledPushedImage;
		Image* toggledDisabledImage;
		hstr toggledNormalImageName;
		hstr toggledHoverImageName;
		hstr toggledPushedImageName;
		hstr toggledDisabledImageName;

		void _draw();
		
		bool _mouseUp(april::Key keyCode);

	private:
		static harray<PropertyDescription> _propertyDescriptions;

	};
}

#endif

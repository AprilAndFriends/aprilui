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
/// Defines an image button.

#ifndef APRILUI_IMAGE_BUTTON_H
#define APRILUI_IMAGE_BUTTON_H

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "Image.h"
#include "ObjectButtonBase.h"
#include "ObjectImageBox.h"

namespace aprilui
{
	class apriluiExport ImageButton : public ImageBox, public ButtonBase
	{
	public:
		ImageButton(chstr name, grect rect);
		~ImageButton();
		inline hstr getClassName() const { return "ImageButton"; }

		static Object* createInstance(chstr name, grect rect);
		
		HL_DEFINE_GET(Image*, hoverImage, HoverImage);
		HL_DEFINE_GET(Image*, pushedImage, PushedImage);
		HL_DEFINE_GET(Image*, disabledImage, DisabledImage);
		HL_DEFINE_GET(hstr, hoverImageName, HoverImageName);
		HL_DEFINE_GET(hstr, pushedImageName, PushedImageName);
		HL_DEFINE_GET(hstr, disabledImageName, DisabledImageName);
		HL_DEFINE_GET(Image*, normalImage, Image);
		void setImage(Image* value);
		hstr getName();
		bool isCursorInside();
		int getFocusIndex();
		Object* getParent();
		Dataset* getDataset();
		void setHoverImage(Image* image);
		void setPushedImage(Image* image);
		void setDisabledImage(Image* name);
		void setHoverImageByName(chstr name);
		void setPushedImageByName(chstr name);
		void setDisabledImageByName(chstr name);
		
		harray<PropertyDescription> getPropertyDescriptions();

		/// @brief Optimized version.
		bool trySetHoverImageByName(chstr name);
		/// @brief Optimized version.
		bool trySetPushedImageByName(chstr name);
		/// @brief Optimized version.
		bool trySetDisabledImageByName(chstr name);
		
		harray<Image*> getUsedImages();

		bool triggerEvent(chstr type, april::Key keyCode);
		bool triggerEvent(chstr type, april::Key keyCode, chstr string);
		bool triggerEvent(chstr type, april::Key keyCode, gvec2 position, chstr string = "", void* userData = NULL);
		bool triggerEvent(chstr type, april::Button buttonCode, chstr string, void* userData = NULL);
		bool triggerEvent(chstr type, chstr string, void* userData = NULL);
		bool triggerEvent(chstr type, void* userData = NULL);

		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);

		bool onMouseDown(april::Key keyCode);
		bool onMouseUp(april::Key keyCode);
		bool onMouseMove();
		bool onButtonDown(april::Button buttonCode);
		bool onButtonUp(april::Button buttonCode);
		void mouseCancel();
		
	protected:
		Image* normalImage;
		Image* hoverImage;
		Image* pushedImage;
		Image* disabledImage;
		hstr normalImageName;
		hstr hoverImageName;
		hstr pushedImageName;
		hstr disabledImageName;
		
		void update(float timeDelta);
		void OnDraw();
		
	private:
		static harray<PropertyDescription> _propertyDescriptions;

	};
}

#endif

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
		static Object* createInstance(chstr name, grect rect);
		
		hstr getName();
		bool isCursorInside();
		Image* getPushedImage() { return this->mPushedImage; }
		void setPushedImage(Image* image) { this->mPushedImage = image; }
		void setPushedImageByName(chstr image);
		Image* getHoverImage() { return this->mHoverImage; }
		void setHoverImage(Image* image) { this->mHoverImage = image; }
		void setHoverImageByName(chstr image);
		Image* getDisabledImage() { return this->mDisabledImage; }
		void setDisabledImage(Image* image) { this->mDisabledImage = image; }
		void setDisabledImageByName(chstr image);
		Image* getImage() { return this->mNormalImage; };
		void setImage(Image* value);
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
		Image* mNormalImage;
		Image* mHoverImage;
		Image* mPushedImage;
		Image* mDisabledImage;
		
		void update(float k);
		void OnDraw();
		
	};
}

#endif

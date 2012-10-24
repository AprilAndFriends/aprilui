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
		
		Image* getHoverImage() { return this->mHoverImage; }
		void setHoverImage(Image* image);
		hstr getHoverImageName() { return this->mHoverImageName; }
		void setHoverImageByName(chstr image);
		Image* getPushedImage() { return this->mPushedImage; }
		void setPushedImage(Image* image);
		hstr getPushedImageName() { return this->mPushedImageName; }
		void setPushedImageByName(chstr image);
		Image* getDisabledImage() { return this->mDisabledImage; }
		void setDisabledImage(Image* image);
		hstr getDisabledImageName() { return this->mDisabledImageName; }
		void setDisabledImageByName(chstr image);

		Image* getImage() { return this->mNormalImage; };
		void setImage(Image* value);
		Object* getParent();
		Dataset* getDataset();
		
		/// @brief Optimized version.
		bool trySetHoverImageByName(chstr name);
		/// @brief Optimized version.
		bool trySetPushedImageByName(chstr name);
		/// @brief Optimized version.
		bool trySetDisabledImageByName(chstr name);

		hstr getProperty(chstr name, bool* property_exists = NULL);
		bool setProperty(chstr name, chstr value);

		bool onMouseDown(int button);
		bool onMouseUp(int button);
		void onMouseMove();
		void cancelMouseDown();
		
	protected:
		Image* mNormalImage;
		Image* mHoverImage;
		Image* mPushedImage;
		Image* mDisabledImage;
		hstr mNormalImageName;
		hstr mHoverImageName;
		hstr mPushedImageName;
		hstr mDisabledImageName;
		
		void update(float k);
		void OnDraw();
		
	};
}

#endif

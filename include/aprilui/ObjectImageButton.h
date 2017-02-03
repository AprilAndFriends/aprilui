/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifndef APRILUI_IMAGE_BUTTON_H
#define APRILUI_IMAGE_BUTTON_H

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "Image.h"
#include "ObjectImageBox.h"

namespace aprilui
{
	class apriluiExport ImageButton : public ImageBox
	{
	public:
		ImageButton(chstr name, grect rect);
		ImageButton(chstr name); // aprilui trunk compatibility

		Image* getPushedImage() { return mPushedImage; }
		void setPushedImage(Image* image) { mPushedImage = image; }
		void setPushedImageByName(chstr image);
		Image* getHoverImage() { return mHoverImage; }
		void setHoverImage(Image* image) { mHoverImage = image; }
		void setHoverImageByName(chstr image);
		Image* getDisabledImage() { return mDisabledImage; }
		void setDisabledImage(Image* image) { mDisabledImage = image; }
		void setDisabledImageByName(chstr image);
		Image* getImage() { return mNormalImage; };
		void setImage(Image* value);
		
		// compatibility methods
		void trySetPushedImageByName(chstr image) { setPushedImageByName(image); }
		void trySetHoverImageByName(chstr image) { setHoverImageByName(image); }
		void trySetDisabledImageByName(chstr image) { setDisabledImageByName(image); }

		bool OnMouseDown(float x, float y, int button);
		bool OnMouseUp(float x, float y, int button);
		bool OnMouseMove(float x, float y);
		
		bool setProperty(chstr name, chstr value);
		
	protected:
		Image* mPushedImage;
		Image* mHoverImage;
		Image* mNormalImage;
		Image* mDisabledImage;
		bool mPushed;
		
		void update(float k);
		void OnDraw(gvec2 offset = gvec2());
		
	};
}

#endif

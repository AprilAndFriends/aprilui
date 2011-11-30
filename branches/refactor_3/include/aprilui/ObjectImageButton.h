/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
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
		
		bool isCursorInside();
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
		
		bool OnMouseDown(float x, float y, int button);
		bool OnMouseUp(float x, float y, int button);
		void OnMouseMove(float x, float y);
		
		void setProperty(chstr name, chstr value);
		
	protected:
		Image* mPushedImage;
		Image* mHoverImage;
		Image* mNormalImage;
		Image* mDisabledImage;
		
		void update(float k);
		void OnDraw(gvec2 offset = gvec2());
		
	};
}

#endif
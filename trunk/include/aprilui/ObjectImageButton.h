/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_IMAGE_BUTTON_H
#define APRILUI_IMAGE_BUTTON_H

#include <hltypes/hstring.h>

#include "AprilUIExport.h"
#include "Image.h"
#include "ObjectImageBox.h"

namespace AprilUI
{
	class AprilUIExport ImageButton : public ImageBox
	{
	protected:
		Image *mPushedImage,*mHoverImage,*mNormalImage,*mDisabledImage;
		bool mPushed;
		
		void OnUpdate(float k);
	public:
		
		ImageButton(chstr name,float x,float y,float w,float h);
		Image* getPushedImage() { return mPushedImage; }
		Image* getHoverImage() { return mHoverImage; }
		Image* getDisabledImage() { return mDisabledImage; }
		void setPushedImage(Image* image);
		void setHoverImage(Image* image);
		void setDisabledImage(Image* image);
		void setPushedImageByName(chstr image);
		void setHoverImageByName(chstr image);
		void setDisabledImageByName(chstr image);
		void OnDraw(float offset_x,float offset_y);
		Image* getImage() { return mNormalImage; };
		void setImage(Image* image);
		void setEnabled(bool enabled);
		
		bool OnMouseDown(float x,float y,int button);
		bool OnMouseUp(float x,float y,int button);
		void OnMouseMove(float x,float y);
		
		void setProperty(chstr name,chstr value);
	};
	
}

#endif

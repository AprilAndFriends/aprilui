/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_IMAGE_BOX_H
#define APRILUI_IMAGE_BOX_H

#include <hltypes/hstring.h>

#include "AprilUIExport.h"
#include "Object.h"

namespace AprilUI
{
	class Image;
	
	class AprilUIExport ImageBox : public Object
	{
	protected:
		Image* mImage;
		hstr mImageName;
		
		void OnDraw(float offset_x,float offset_y);
	public:
		ImageBox(chstr name,float x,float y,float w,float h);
		
		virtual Image* getImage() { return mImage; };
		virtual void setImage(Image* image);
		void setImageByName(chstr image);
		hstr getImageName() { return mImageName; }
		void notifyEvent(chstr event_name,void* params);
		
		void resizeToFitImage();
		
		void setProperty(chstr name,chstr value);
		bool OnMouseDown(float x,float y,int button);
		bool OnMouseUp(float x,float y,int button);
	};
	
}

#endif

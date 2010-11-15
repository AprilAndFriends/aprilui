/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_IMAGE_BOX_H
#define APRILUI_IMAGE_BOX_H

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "AprilUIExport.h"
#include "Object.h"

namespace AprilUI
{
	class Image;
	
	class AprilUIExport ImageBox : public Object
	{
	public:
		ImageBox(chstr name, grect rect);
		
		virtual Image* getImage() { return mImage; };
		virtual void setImage(Image* value);
		hstr getImageName() { return mImageName; }
		void setImageByName(chstr name);
		void notifyEvent(chstr name, void* params);
		
		void resizeToFitImage();
		
		void setProperty(chstr name, chstr value);
		bool OnMouseDown(float x, float y, int button);
		bool OnMouseUp(float x, float y, int button);
		
	protected:
		Image* mImage;
		hstr mImageName;
		
		void OnDraw(gvec2 offset = gvec2());
		
	};
}

#endif

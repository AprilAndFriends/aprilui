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

#include "apriluiExport.h"
#include "Object.h"

namespace aprilui
{
	class Image;
	
	class apriluiExport ImageBox : public Object
	{
	public:
		ImageBox(chstr name, grect rect);
		
		virtual Image* getImage() { return mImage; };
		virtual void setImage(Image* value);
		hstr getImageName() { return mImageName; }
		void setImageByName(chstr name);
		void notifyEvent(chstr name, void* params);
		
		void resizeToFitImage();
		
		hstr getProperty(chstr name, bool* property_exists = NULL);
		bool setProperty(chstr name, chstr value);
		bool onMouseDown(float x, float y, int button);
		bool onMouseUp(float x, float y, int button);
		
	protected:
		Image* mImage;
		hstr mImageName;
		
		void OnDraw();
		
	};
}

#endif

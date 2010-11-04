/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_COMPOSITE_IMAGE_H
#define APRILUI_COMPOSITE_IMAGE_H

#include <gtypes/Rectangle.h>
#include <hltypes/harray.h>

#include "Image.h"

#include "AprilUIExport.h"

namespace AprilUI
{
	class AprilUIExport CompositeImage : public Image
	{
	public:
		struct ImageRef
		{
			Image* image;
			grect rect;
			float x, y, w, h;
		};
	public:
		CompositeImage(chstr name, float sourceW, float sourceH);
		CompositeImage(chstr name, CompositeImage& base);
		
		void addImageRef(Image* img, float x, float y, float w, float h);
		
		void draw(float dx, float dy, float dw, float dh, float r, float g, float b, float a);
		void draw(float centerx, float centery, float dw, float dh, float angle, float r, float g, float b, float a);
		
		const harray<ImageRef>& getImageList() { return mImages; }
		
	protected:
		harray<ImageRef> mImages;
		
	};
	
}
#endif

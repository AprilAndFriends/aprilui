/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <math.h>

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <hltypes/util.h>

#include "CompositeImage.h"

namespace AprilUI
{
	CompositeImage::CompositeImage(chstr name, float sourceW, float sourceH) :
		Image(0, name, 0, 0, sourceW, sourceH)
	{
		
	}
	
	CompositeImage::CompositeImage(chstr name, CompositeImage& base) :
		Image(0, name, 0, 0, base.getSourceW(), base.getSourceH())
	{
		foreach(ImageRef, it, base.mImages)
		{
			addImageRef((*it).image, (*it).x, (*it).y, (*it).w, (*it).h);
		}
	}
	
	void CompositeImage::addImageRef(Image* image, float x, float y, float w, float h)
	{
		ImageRef reference;
		reference.image = image;
		//reference.
		reference.x = x;
		reference.y = y;
		reference.w = w;
		reference.h = h;
		mImages.push_back(reference);
	}

	
	void CompositeImage::draw(float dx, float dy, float dw, float dh, float r, float g, float b, float a)
	{
		float factorWidth = dw / mSourceW;
		float factorHeight = dh / mSourceH;
		foreach(ImageRef, it, mImages)
		{
			(*it).image->draw(dx + (*it).x * factorWidth, dy + (*it).y * factorHeight, (*it).w * factorWidth,(*it).h * factorHeight, r, g, b, a);
		}
	}
	
	void CompositeImage::draw(float centerx, float centery, float dw, float dh, float angle, float r, float g, float b, float a)
	{
		float factorWidth = dw / mSourceW;
		float factorHeight = dh / mSourceH;
		foreach(ImageRef, it, mImages)
		{
			(*it).image->draw(centerx - dw / 2 + (*it).x * factorWidth + (*it).w / 2, centery - dh / 2 + (*it).y * factorHeight + (*it).h / 2,
				(*it).w * factorWidth, (*it).h * factorHeight, angle, r, g, b, a);
		}
	}

}

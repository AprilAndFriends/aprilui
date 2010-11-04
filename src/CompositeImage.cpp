/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
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
			addImageRef((*it).image, (*it).rect);
		}
	}
	
	void CompositeImage::addImageRef(Image* image, float x, float y, float w, float h)
	{
		addImageRef(image, grect(x, y, w,h));
	}

	void CompositeImage::addImageRef(Image* image, grect rect)
	{
		ImageRef reference;
		reference.image = image;
		reference.rect = rect;
		mImages.push_back(reference);
	}

	void CompositeImage::draw(float x, float y, float w, float h, float r, float g, float b, float a)
	{
		float wf = w / mSourceW;
		float hf = h / mSourceH;
		foreach(ImageRef, it, mImages)
		{
			(*it).image->draw(x + (*it).rect.x * wf, y + (*it).rect.y * hf, (*it).rect.w * wf, (*it).rect.h * hf, r, g, b, a);
		}
	}
	
	void CompositeImage::draw(float x, float y, float w, float h, float r, float g, float b, float a, float angle)
	{
		float wf = w / mSourceW;
		float hf = h / mSourceH;
		foreach(ImageRef, it, mImages)
		{
			(*it).image->draw(x + (*it).rect.x * wf, y + (*it).rect.y * hf,
				(*it).rect.w * wf, (*it).rect.h * hf, r, g, b, a, angle);
		}
	}

}

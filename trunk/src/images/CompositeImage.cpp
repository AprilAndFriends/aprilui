/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <math.h>

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/util.h>

#include "CompositeImage.h"

namespace aprilui
{
	CompositeImage::CompositeImage(chstr name, float w, float h) : Image(0, name, grect(0, 0, w, h))
	{
		
	}
	
	CompositeImage::CompositeImage(chstr name, CompositeImage& base) :
		Image(0, name, grect(0, 0, base.getSrcRect().getSize()))
	{
		foreach (ImageRef, it, base.mImages)
		{
			addImageRef((*it).image, (*it).rect);
		}
	}
	
	CompositeImage::~CompositeImage()
	{
	}

	void CompositeImage::addImageRef(Image* image, grect rect)
	{
		ImageRef reference;
		reference.image = image;
		reference.rect = rect;
		mImages += reference;
	}

	void CompositeImage::draw(grect rect, april::Color color)
	{
		float wf = rect.w / mSrcRect.w;
		float hf = rect.h / mSrcRect.h;
		grect drawRect;
		foreach (ImageRef, it, mImages)
		{
			drawRect.x = rect.x + (*it).rect.x * wf;
			drawRect.y = rect.y + (*it).rect.y * hf;
			drawRect.w = (*it).rect.w * wf;
			drawRect.h = (*it).rect.h * hf;
			(*it).image->draw(drawRect, color);
		}
	}
	
}

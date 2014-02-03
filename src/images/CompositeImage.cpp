/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <math.h>

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hltypesUtil.h>

#include "CompositeImage.h"

namespace aprilui
{
	CompositeImage::CompositeImage(chstr name, float w, float h) : Image(0, name, grect(0, 0, w, h))
	{
		
	}
	
	CompositeImage::CompositeImage(chstr name, CompositeImage& base) :
		Image(0, name, grect(0, 0, base.getSrcRect().getSize()))
	{
		foreach (ImageRef, it, base.images)
		{
			this->addImageRef((*it).image, (*it).rect);
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
		this->images += reference;
	}
    
    void CompositeImage::clearImages()
    {
        this->images.clear();
    }

	void CompositeImage::draw(grect rect, april::Color color)
	{
		gvec2 sf = rect.getSize() / this->srcRect.getSize();
		grect drawRect;
		foreach (ImageRef, it, this->images)
		{
			drawRect.set(rect.getPosition() + (*it).rect.getPosition() * sf, (*it).rect.getSize() * sf);
			(*it).image->draw(drawRect, color);
		}
	}
	
}

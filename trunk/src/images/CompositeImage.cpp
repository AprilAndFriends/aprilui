/// @file
/// @version 3.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <math.h>

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hlog.h>
#include <hltypes/hltypesUtil.h>

#include "aprilui.h"
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
		if (color.a == 0)
		{
			return;
		}
		gvec2 sf = rect.getSize() / this->srcRect.getSize();
		grect drawRect;
		foreach (ImageRef, it, this->images)
		{
			drawRect.set(rect.getPosition() + (*it).rect.getPosition() * sf, (*it).rect.getSize() * sf);
			(*it).image->draw(drawRect, color);
		}
	}
	
	void CompositeImage::draw(harray<april::TexturedVertex> vertices, april::Color color)
	{
		hlog::warn(aprilui::logTag, "CompositeImage::draw(harray<april::TexturedVertex>, april::Color) is currently not supported!");
	}
	
}

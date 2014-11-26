/// @file
/// @version 4.0
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
	CompositeImage::CompositeImage(chstr name, float w, float h) : Image(0, name, grect(0, 0, w, h)), restoreClipRects(true)
	{
	}
	
	CompositeImage::CompositeImage(chstr name, CompositeImage& base) : Image(0, name, grect(0, 0, base.getSrcRect().getSize()))
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
		// using separate loops for performance reasons
		if (this->clipRect.w == 0.0f || this->clipRect.h == 0.0f)
		{
			foreach (ImageRef, it, this->images)
			{
				drawRect.set(rect.getPosition() + (*it).rect.getPosition() * sf, (*it).rect.getSize() * sf);
				(*it).image->draw(drawRect, color);
			}
		}
		else
		{
			gvec2 cf;
			grect oldClipRect;
			grect clipRect;
			foreach (ImageRef, it, this->images)
			{
				clipRect.set(0.0f, 0.0f, (*it).rect.getSize());
				clipRect.clip(this->clipRect - (*it).rect.getPosition());
				cf = (*it).image->getSrcSize() / (*it).rect.getSize();
				clipRect.set(clipRect.getPosition() * cf, clipRect.getSize() * cf);
				if (clipRect.w > 0.0f && clipRect.h > 0.0f)
				{
					(*it).image->setClipRect(clipRect);
					oldClipRect = (*it).image->getClipRect();
					drawRect.set(rect.getPosition() + (*it).rect.getPosition() * sf, (*it).rect.getSize() * sf);
					(*it).image->draw(drawRect, color);
					if (this->restoreClipRects)
					{
						(*it).image->setClipRect(oldClipRect);
					}
				}
			}
		}
	}
	
	void CompositeImage::draw(harray<april::TexturedVertex> vertices, april::Color color)
	{
		hlog::warn(aprilui::logTag, "CompositeImage::draw(harray<april::TexturedVertex>, april::Color) is not supported!");
	}
	
}

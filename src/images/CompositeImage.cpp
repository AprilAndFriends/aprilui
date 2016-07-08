/// @file
/// @version 4.1
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
#include "Image.h"

namespace aprilui
{
	harray<PropertyDescription> CompositeImage::_propertyDescriptions;

	CompositeImage::CompositeImage(chstr name, gvec2 size) : BaseImage(name), restoreClipRects(true)
	{
		this->size = size;
	}
	
	// DEPRECATED
	CompositeImage::CompositeImage(chstr name, const CompositeImage& other) : BaseImage(other)
	{
		this->name = name;
		this->size = other.size;
		this->images = other.images;
		this->restoreClipRects = other.restoreClipRects;
	}

	// DEPRECATED
	CompositeImage::CompositeImage(const CompositeImage& other, chstr name) : BaseImage(other)
	{
		this->name = name;
		this->size = other.size;
		this->images = other.images;
		this->restoreClipRects = other.restoreClipRects;
	}

	CompositeImage::CompositeImage(const CompositeImage& other) : BaseImage(other)
	{
		this->size = other.size;
		this->images = other.images;
		this->restoreClipRects = other.restoreClipRects;
	}

	CompositeImage::~CompositeImage()
	{
	}

	harray<PropertyDescription> CompositeImage::getPropertyDescriptions() const
	{
		if (CompositeImage::_propertyDescriptions.size() == 0)
		{
			CompositeImage::_propertyDescriptions += PropertyDescription("restore_clip_rects", PropertyDescription::BOOL);
		}
		return (CompositeImage::_propertyDescriptions + BaseImage::getPropertyDescriptions());
	}

	void CompositeImage::addImageRef(BaseImage* image, grect rect)
	{
		this->images += ImageRef(image, rect);
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
		gvec2 sf = rect.getSize() / this->size;
		grect drawRect;
		// using separate loops for performance reasons
		if (this->clipRect.w == 0.0f || this->clipRect.h == 0.0f)
		{
			foreach (ImageRef, it, this->images)
			{
				drawRect.set(rect.getPosition() + (*it).second.getPosition() * sf, (*it).second.getSize() * sf);
				(*it).first->draw(drawRect, color);
			}
		}
		else
		{
			gvec2 cf;
			grect oldClipRect;
			grect clipRect;
			foreach (ImageRef, it, this->images)
			{
				clipRect.set(0.0f, 0.0f, (*it).second.getSize());
				clipRect.clip(this->clipRect - (*it).second.getPosition());
				cf = (*it).first->getSrcSize() / (*it).second.getSize();
				clipRect.set(clipRect.getPosition() * cf, clipRect.getSize() * cf);
				if (clipRect.w > 0.0f && clipRect.h > 0.0f)
				{
					oldClipRect = (*it).first->getClipRect();
					(*it).first->setClipRect(clipRect);
					drawRect.set(rect.getPosition() + (*it).second.getPosition() * sf, (*it).second.getSize() * sf);
					(*it).first->draw(drawRect, color);
					if (this->restoreClipRects)
					{
						(*it).first->setClipRect(oldClipRect);
					}
				}
			}
		}
	}
	
	void CompositeImage::draw(harray<april::TexturedVertex> vertices, april::Color color)
	{
		hlog::warn(logTag, "CompositeImage::draw(harray<april::TexturedVertex>, april::Color) is not supported!");
	}
	
	hstr CompositeImage::getProperty(chstr name)
	{
		if (name == "restore_clip_rects")	return this->isRestoreClipRects();
		return BaseImage::getProperty(name);
	}
	
	bool CompositeImage::setProperty(chstr name, chstr value)
	{
		if		(name == "restore_clip_rects")	this->setRestoreClipRects(value);
		else return BaseImage::setProperty(name, value);
		return true;
	}

}

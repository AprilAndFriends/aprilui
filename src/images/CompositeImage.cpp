/// @file
/// @version 5.0
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
	hmap<hstr, PropertyDescription> CompositeImage::_propertyDescriptions;
	hmap<hstr, PropertyDescription::Accessor*> CompositeImage::_getters;
	hmap<hstr, PropertyDescription::Accessor*> CompositeImage::_setters;

	CompositeImage::CompositeImage(chstr name, cgvec2f size) :
		BaseImage(name),
		restoreClipRects(true)
	{
		this->size = size;
	}
	
	CompositeImage::CompositeImage(const CompositeImage& other) :
		BaseImage(other)
	{
		this->size = other.size;
		this->images = other.images;
		this->restoreClipRects = other.restoreClipRects;
	}

	CompositeImage::~CompositeImage()
	{
	}

	hmap<hstr, PropertyDescription>& CompositeImage::getPropertyDescriptions() const
	{
		if (CompositeImage::_propertyDescriptions.size() == 0)
		{
			CompositeImage::_propertyDescriptions = BaseImage::getPropertyDescriptions();
			CompositeImage::_propertyDescriptions["restore_clip_rects"] = PropertyDescription("restore_clip_rects", PropertyDescription::Type::Bool);
		}
		return CompositeImage::_propertyDescriptions;
	}

	hmap<hstr, PropertyDescription::Accessor*>& CompositeImage::_getGetters() const
	{
		if (CompositeImage::_getters.size() == 0)
		{
			CompositeImage::_getters = BaseImage::_getGetters();
			CompositeImage::_getters["restore_clip_rects"] = new PropertyDescription::Get<CompositeImage, bool>(&CompositeImage::isRestoreClipRects);
		}
		return CompositeImage::_getters;
	}

	hmap<hstr, PropertyDescription::Accessor*>& CompositeImage::_getSetters() const
	{
		if (CompositeImage::_setters.size() == 0)
		{
			CompositeImage::_setters = BaseImage::_getSetters();
			CompositeImage::_setters["restore_clip_rects"] = new PropertyDescription::Set<CompositeImage, bool>(&CompositeImage::setRestoreClipRects);
		}
		return CompositeImage::_setters;
	}
	
	void CompositeImage::addImageRef(BaseImage* image, cgrectf rect)
	{
		this->images += ImageRef(image, rect);
	}
	
	void CompositeImage::clearImages()
	{
		this->images.clear();
	}
	
	void CompositeImage::draw(cgrectf rect, const april::Color& color)
	{
		if (color.a == 0)
		{
			return;
		}
		gvec2f sf = rect.getSize() / this->size;
		grectf drawRect;
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
			gvec2f cf;
			grectf oldClipRect;
			grectf clipRect;
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
	
	void CompositeImage::draw(const harray<april::TexturedVertex>& vertices, const april::Color& color)
	{
		hlog::warn(logTag, "CompositeImage::draw(harray<april::TexturedVertex>, april::Color) is not supported!");
	}
	
}

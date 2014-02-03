/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <april/RenderSystem.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "Dataset.h"
#include "Image.h"
#include "ObjectImageBox.h"

namespace aprilui
{
	ImageBox::ImageBox(chstr name, grect rect) : Object(name, rect)
	{
		this->image = NULL;
	}

	ImageBox::~ImageBox()
	{
	}

	Object* ImageBox::createInstance(chstr name, grect rect)
	{
		return new ImageBox(name, rect);
	}

	void ImageBox::setImage(Image* image)
	{
		this->image = image;
		if (image != NULL)
		{
			grect rect = image->getSrcRect();
			if (this->rect.w == -1)
			{
				this->rect.w = rect.w * aprilui::getDefaultScale();
				this->center.x = rect.w * 0.5f;
			}
			if (this->rect.h == -1)
			{
				this->rect.h = rect.h * aprilui::getDefaultScale();
				this->center.y = this->rect.h * 0.5f;
			}
			this->imageName = image->getFullName();
		}
		else
		{
			this->imageName = APRILUI_IMAGE_NAME_NULL;
		}
	}

	void ImageBox::setImageByName(chstr name)
	{
		this->setImage(this->dataset->getImage(name));
	}

	bool ImageBox::trySetImageByName(chstr name)
	{
		if (this->imageName != name)
		{
			// using c/p code because of performance reasons
			this->setImage(this->dataset->getImage(name));
			return true;
		}
		return false;
	}
	
	void ImageBox::OnDraw()
	{
		if (this->image == NULL)
		{
			this->image = this->dataset->getImage(APRILUI_IMAGE_NAME_NULL);
		}
		april::Color color = this->_getDrawColor();
		color.a = (unsigned char)(color.a * this->_getDisabledAlphaFactor());
		if (!aprilui::isDebugEnabled())
		{
			this->image->draw(this->_getDrawRect(), color);
		}
		else
		{
			grect rect = this->_getDrawRect();
			april::rendersys->drawFilledRect(rect, april::Color(april::Color::Black, 32));
			april::rendersys->drawRect(rect, april::Color(april::Color::White, 64));
			this->image->draw(rect, color);
		}
		Object::OnDraw();
	}
	
	void ImageBox::resizeToFitImage()
	{
		if (this->image != NULL)
		{
			this->rect.setSize(this->image->getSrcRect().getSize() * aprilui::getDefaultScale());
			this->resetCenter();
		}
	}

	void ImageBox::notifyEvent(chstr name, void* params)
	{	
		if (name == "UpdateImage")
		{
			this->setImageByName(this->imageName);
		}
		Object::notifyEvent(name, params);
	}
	
	hstr ImageBox::getProperty(chstr name, bool* propertyExists)
	{
		if (propertyExists != NULL)
		{
			*propertyExists = true;
		}
		if (name == "image")	return this->getImageName();
		return Object::getProperty(name, propertyExists);
	}

	bool ImageBox::setProperty(chstr name, chstr value)
	{
		if (name == "image")	this->setImageByName(value);
		else return Object::setProperty(name, value);
		return true;
	}

	bool ImageBox::onMouseDown(april::Key keyCode)
	{
		if (Object::onMouseDown(keyCode))
		{
			return true;
		}
		if (this->isCursorInside())
		{
			this->triggerEvent("MouseDown", keyCode);
			return true;
		}
		return false;
	}

	bool ImageBox::onMouseUp(april::Key keyCode)
	{
		if (Object::onMouseUp(keyCode))
		{
			return true;
		}
		if (this->isCursorInside())
		{
			this->triggerEvent("Click", keyCode);
			return true;
		}
		return false;
	}
}

/// @file
/// @version 3.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/RenderSystem.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "Dataset.h"
#include "Image.h"
#include "ObjectImageBox.h"

namespace aprilui
{
	harray<PropertyDescription> ImageBox::_propertyDescriptions;

	ImageBox::ImageBox(chstr name, grect rect) : Object(name, rect)
	{
		this->image = NULL;
		this->debugColor = april::Color::Clear;
	}

	ImageBox::~ImageBox()
	{
	}

	Object* ImageBox::createInstance(chstr name, grect rect)
	{
		return new ImageBox(name, rect);
	}

	harray<PropertyDescription> ImageBox::getPropertyDescriptions()
	{
		if (ImageBox::_propertyDescriptions.size() == 0)
		{
			ImageBox::_propertyDescriptions += PropertyDescription("image", PropertyDescription::STRING);
		}
		return (Object::getPropertyDescriptions() + ImageBox::_propertyDescriptions);
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
	
	harray<Image*> ImageBox::getUsedImages()
	{
		harray<Image*> images;
		if (this->image != NULL)
		{
			images += this->image;
		}
		return images;
	}
	
	void ImageBox::_draw()
	{
		if (this->image == NULL)
		{
			this->image = this->dataset->getImage(APRILUI_IMAGE_NAME_NULL);
		}
		april::Color color = this->_getDrawColor();
		color.a = (unsigned char)(color.a * this->_getDisabledAlphaFactor());
		this->image->draw(this->_getDrawRect(), color);
		Object::_draw();
	}
	
	void ImageBox::resizeToFitImage()
	{
		if (this->image != NULL)
		{
			this->rect.setSize(this->image->getSrcRect().getSize() * aprilui::getDefaultScale());
			this->resetCenter();
		}
	}

	hstr ImageBox::getProperty(chstr name)
	{
		if (name == "image")	return this->getImageName();
		return Object::getProperty(name);
	}

	bool ImageBox::setProperty(chstr name, chstr value)
	{
		if (name == "image")	this->setImageByName(value);
		else return Object::setProperty(name, value);
		return true;
	}
}

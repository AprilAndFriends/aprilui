/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.64
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
	ImageBox::ImageBox(chstr name, grect rect) :
		Object(name, rect)
	{
		this->mImage = NULL;
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
		this->mImage = image;
		if (image != NULL)
		{
			grect rect = image->getSrcRect();
			if (this->mRect.w == -1)
			{
				this->mRect.w = rect.w * aprilui::getDefaultScale();
				this->mCenter.x = mRect.w * 0.5f;
			}
			if (this->mRect.h == -1)
			{
				this->mRect.h = rect.h * aprilui::getDefaultScale();
				this->mCenter.y = this->mRect.h * 0.5f;
			}
			this->mImageName = image->getFullName();
		}
		else
		{
			this->mImageName = APRILUI_IMAGE_NAME_NULL;
		}
	}

	void ImageBox::setImageByName(chstr name)
	{
		this->setImage(this->mDataset->getImage(name));
	}

	bool ImageBox::trySetImageByName(chstr name)
	{
		if (this->mImageName != name)
		{
			// using c/p code because of performance reasons
			this->setImage(this->mDataset->getImage(name));
			return true;
		}
		return false;
	}
	
	void ImageBox::OnDraw()
	{
		if (this->mImage == NULL)
		{
			this->mImage = this->mDataset->getImage(APRILUI_IMAGE_NAME_NULL);
		}
		april::Color color = this->_getDrawColor();
		color.a = (unsigned char)(color.a * this->_getDisabledAlphaFactor());
		if (!aprilui::isDebugEnabled())
		{
			this->mImage->draw(this->_getDrawRect(), color);
		}
		else
		{
			grect rect = this->_getDrawRect();
			april::rendersys->drawFilledRect(rect, april::Color(april::Color::Black, 32));
			april::rendersys->drawRect(rect, april::Color(april::Color::White, 64));
			this->mImage->draw(rect, color);
		}
		Object::OnDraw();
	}
	
	void ImageBox::resizeToFitImage()
	{
		if (this->mImage != NULL)
		{
			this->mRect.setSize(this->mImage->getSrcRect().getSize() * aprilui::getDefaultScale());
			this->resetCenter();
		}
	}

	void ImageBox::notifyEvent(chstr name, void* params)
	{	
		if (name == "UpdateImage")
		{
			this->setImageByName(this->mImageName);
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

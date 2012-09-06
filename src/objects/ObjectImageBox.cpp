/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.91
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
			Dataset* dataset = image->getDataset();
			this->mImageName = dataset == mDataset ? image->getName() : image->getFullName();
			grect rect = image->getSrcRect();
			if (this->mRect.w == -1)
			{
				this->mRect.w = rect.w * aprilui::getDefaultScale();
				this->mCenter.x = mRect.w / 2;
			}
			if (this->mRect.h == -1)
			{
				this->mRect.h = rect.h * aprilui::getDefaultScale();
				this->mCenter.y = this->mRect.h / 2;
			}
		}
		else
		{
			this->mImageName = "null";
		}
	}

	void ImageBox::setImageByName(chstr image)
	{
		this->setImage(this->mDataset->getImage(image));
		this->mImageName = image;
	}
	
	void ImageBox::notifyEvent(chstr name, void* params)
	{	
		if (name == "UpdateImage")
		{
			this->setImageByName(this->mImageName);
		}
		Object::notifyEvent(name, params);
	}
	
	void ImageBox::resizeToFitImage()
	{
		if (this->mImage != NULL)
		{
			this->mRect.setSize(this->mImage->getSrcRect().getSize() * aprilui::getDefaultScale());
			this->resetCenter();
		}
	}

	void ImageBox::OnDraw()
	{
		if (this->mImage == NULL)
		{
			this->mImage = this->mDataset->getImage("null");
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
			april::Color backColor = april::Color(APRIL_COLOR_BLACK, 96);
			april::rendersys->drawFilledRect(rect, backColor);
			april::rendersys->drawRect(rect, april::Color(APRIL_COLOR_WHITE, 96));
			this->mImage->draw(rect, color);
		}
		Object::OnDraw();
	}
	
	hstr ImageBox::getProperty(chstr name, bool* property_exists)
	{
		if (property_exists != NULL)
		{
			*property_exists = true;
		}
		if (name == "image")	return this->getImageName();
		return Object::getProperty(name, property_exists);
	}

	bool ImageBox::setProperty(chstr name, chstr value)
	{
		if (name == "image")	this->setImageByName(value);
		else return Object::setProperty(name, value);
		return true;
	}

	bool ImageBox::onMouseDown(int button)
	{
		if (Object::onMouseDown(button))
		{
			return true;
		}
		if (this->isCursorInside())
		{
			this->triggerEvent("MouseDown", button);
			return true;
		}
		return false;
	}

	bool ImageBox::onMouseUp(int button)
	{
		if (Object::onMouseUp(button))
		{
			return true;
		}
		if (this->isCursorInside())
		{
			this->triggerEvent("Click", button);
			return true;
		}
		return false;
	}
}

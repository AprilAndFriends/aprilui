/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.7
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
		mImage = NULL;
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
		mImage = image;
		if (image != NULL)
		{
			mImageName = image->getName();
			grect rect = image->getSrcRect();
			if (mRect.w == -1)
			{
				mRect.w = rect.w * getDefaultScale();
				mCenter.x = mRect.w / 2;
			}
			if (mRect.h == -1)
			{
				mRect.h = rect.h * getDefaultScale();
				mCenter.y = mRect.h / 2;
			}
		}
		else
		{
			mImageName = "null";
		}
	}

	void ImageBox::setImageByName(chstr image)
	{
		setImage(mDataset->getImage(image));
		mImageName = image;
	}
	
	void ImageBox::notifyEvent(chstr name, void* params)
	{	
		if (name == "UpdateImage")
		{
			setImageByName(mImageName);
		}
		Object::notifyEvent(name, params);
	}
	
	void ImageBox::resizeToFitImage()
	{
		if (mImage != NULL)
		{
			mRect.setSize(mImage->getSrcRect().getSize() * getDefaultScale());
			resetCenter();
		}
	}

	void ImageBox::OnDraw()
	{
		if (mImage == NULL)
		{
			mImage = mDataset->getImage("null");
		}
		april::Color color = _getDrawColor();
		if (!_isDerivedEnabled())
		{
			color.a /= 2;
		}
		if (!aprilui::isDebugEnabled())
		{
			mImage->draw(_getDrawRect(), color);
		}
		else
		{
			grect rect = _getDrawRect();
			april::Color backColor = april::Color(APRIL_COLOR_BLACK, 96);
			april::rendersys->drawFilledRect(rect, backColor);
			april::rendersys->drawRect(rect, april::Color(APRIL_COLOR_WHITE, 96));
			mImage->draw(rect, color);
		}
		Object::OnDraw();
	}
	
	hstr ImageBox::getProperty(chstr name, bool* property_exists)
	{
		if (property_exists != NULL)
		{
			*property_exists = true;
		}
		if (name == "image")	return getImageName();
		return Object::getProperty(name, property_exists);
	}

	bool ImageBox::setProperty(chstr name, chstr value)
	{
		if (name == "image") setImageByName(value);
		else return Object::setProperty(name, value);
		return true;
	}

	bool ImageBox::onMouseDown(int button)
	{
		if (Object::onMouseDown(button))
		{
			return true;
		}
		if (isCursorInside())
		{
            _triggerEvent("MouseDown", button);
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
		if (isCursorInside())
		{
			_triggerEvent("Click", button);
			return true;
		}
		return false;
	}
}

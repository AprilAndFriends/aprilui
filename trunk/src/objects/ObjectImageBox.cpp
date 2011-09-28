/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "Dataset.h"
#include "Image.h"
#include "ObjectImageBox.h"

namespace aprilui
{
	ImageBox::ImageBox(chstr name, grect rect) :
		Object("ImageBox", name, rect)
	{
		mImage = NULL;
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
			}
			if (mRect.h == -1)
			{
				mRect.h = rect.h * getDefaultScale();
			}
			mCenter = mRect.getSize() / 2;
		}
		else
		{
			mImageName = "null";
			mCenter.set(0.0f, 0.0f);
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
		if (mImage)
		{
			setSize(mImage->getSrcRect().getSize() * getDefaultScale());
			setCenter(getSize() / 2);
		}
	}

	void ImageBox::OnDraw()
	{
		if (mImage == NULL)
		{
			mImage = mDataset->getImage("null");
		}
		april::Color color(mColor, getDerivedAlpha());
		if (!isDerivedEnabled())
		{
			color.a /= 2;
		}
		mImage->draw(_getDrawRect(), color);
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

	bool ImageBox::onMouseDown(float x, float y, int button)
	{
		if (Object::onMouseDown(x, y, button))
		{
			return true;
		}
		if (isCursorInside())
		{
            triggerEvent("MouseDown", x, y, button);
			return true;
		}
		return false;
	}

	bool ImageBox::onMouseUp(float x, float y, int button)
	{
		if (Object::onMouseUp(x, y, button))
		{
			return true;
		}
		if (isCursorInside())
		{
			triggerEvent("Click", x, y, button);
			return true;
		}
		return false;
	}
}

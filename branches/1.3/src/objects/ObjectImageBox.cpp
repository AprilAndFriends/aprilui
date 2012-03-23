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
		mImageName = (image != NULL ? image->getName() : "null");
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
	
	void ImageBox::setProperty(chstr name, chstr value)
	{
		Object::setProperty(name, value);
		if (name == "image")	setImageByName(value);
	}

	bool ImageBox::OnMouseDown(float x, float y, int button)
	{
		if (Object::OnMouseDown(x, y, button))
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

	bool ImageBox::OnMouseUp(float x, float y, int button)
	{
		if (Object::OnMouseUp(x, y, button))
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
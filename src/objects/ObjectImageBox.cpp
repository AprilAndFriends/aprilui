/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hstring.h>

#include "aprilui.h"
#include "Dataset.h"
#include "Image.h"
#include "ObjectColoredImageBox.h"

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
			grect rect = image->getSource();
			if (mRect.w == -1)
			{
				mRect.w = rect.w * getDefaultScale();
			}
			if (mRect.h == -1)
			{
				mRect.h = rect.h * getDefaultScale();
			}
		}
		else
		{
			mImageName = image->getName();
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
		if (mImage)
		{
			grect rect = mImage->getSource();
			setSize(rect.w * getDefaultScale(), rect.h * getDefaultScale());
		}
	}

	void ImageBox::OnDraw(gvec2 offset)
	{
		if (mImage == NULL)
		{
			mImage = mDataset->getImage("null");
		}
		float alpha = getDerivedAlpha();
		if (!isDerivedEnabled())
		{
			alpha /= 2;
		}
		april::Color color;
		color.a = (unsigned char)(alpha * 255);
		mImage->draw(_getDrawRect() + offset, color);
		april::rendersys->setBlendMode(april::BM_DEFAULT);
		april::rendersys->setColorMode(april::CM_DEFAULT);
	}

	bool ImageBox::setProperty(chstr name, chstr value)
	{
		Object::setProperty(name, value);
		if (name == "image")	setImageByName(value);
		return 1;
	}

	bool ImageBox::OnMouseDown(float x, float y, int button)
	{
		if (Object::OnMouseDown(x, y, button))
		{
			return true;
		}
		if (isPointInside(x, y))
		{
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
		if (isPointInside(x, y))
		{
			triggerEvent("Click", x, y, 0);
			return true;
		}
		return false;
	}
	
}

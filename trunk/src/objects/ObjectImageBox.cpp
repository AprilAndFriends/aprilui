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
		april::Color color(mColor, getDerivedAlpha());
		if (!_isDerivedEnabled())
		{
			color.a /= 2;
		}
#ifndef _DEBUG
		mImage->draw(_getDrawRect(), color);
#else
		if (!aprilui::isDebugMode())
		{
			mImage->draw(_getDrawRect(), color);
		}
		else
		{
			grect rect = _getDrawRect();
			april::Color drawColor = april::Color(APRIL_COLOR_BLACK, 96);
			april::rendersys->drawColoredQuad(grect(rect.x + 1, rect.y + 1, rect.w - 2, rect.h - 2), drawColor);
			drawColor = april::Color(APRIL_COLOR_WHITE, 96);
			april::rendersys->drawColoredQuad(grect(rect.x, rect.y, rect.w, 1), drawColor);
			april::rendersys->drawColoredQuad(grect(rect.x, rect.y, 1, rect.h), drawColor);
			april::rendersys->drawColoredQuad(grect(rect.x, rect.y + rect.h - 1, rect.w, 1), drawColor);
			april::rendersys->drawColoredQuad(grect(rect.x + rect.w - 1, rect.y, 1, rect.h), drawColor);
			mImage->draw(rect, color);
		}
#endif
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
            _triggerEvent("MouseDown", x, y, button);
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
			_triggerEvent("Click", x, y, button);
			return true;
		}
		return false;
	}
}

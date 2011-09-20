/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "Dataset.h"
#include "Image.h"
#include "ObjectImageButton.h"

namespace aprilui
{
	ImageButton::ImageButton(chstr name, grect rect) :
		ButtonBase(),
		ImageBox(name, rect)
	{
		_setTypeName("ImageButton");
		mNormalImage = NULL;
		mPushedImage = NULL;
		mHoverImage = NULL;
		mDisabledImage = NULL;
	}

	void ImageButton::OnDraw()
	{
		grect rect = _getDrawRect();
		if (!isDerivedEnabled() && mDisabledImage != NULL)
		{
			mDisabledImage->draw(rect, april::Color(mColor, getDerivedAlpha()));
			return;
		}
		if (mPushed && mPushedImage == NULL && isCursorInside())
		{
			mImage->draw(rect, april::Color(mColor * 0.75f, getDerivedAlpha()));
			return;
		}
		ImageBox::OnDraw();
	}

	void ImageButton::update(float k)
	{
		ButtonBase::update(k);
		mImage = mNormalImage;
		if (mImage == NULL)
		{
			mImage = mDataset->getImage("null");
		}
		if (!isDerivedEnabled())
		{
			if (mDisabledImage != NULL)
			{
				mImage = mDisabledImage;
			}
		}
		else if (mHover)
		{
			if (mPushed)
			{
				if (mPushedImage != NULL)
				{
					mImage = mPushedImage;
				}
			}
			else if (mHoverImage != NULL && aprilui::isHoverEffectEnabled())
			{
				mImage = mHoverImage;
			}
		}
		ImageBox::update(k);
	}
	
	bool ImageButton::isCursorInside()
	{
		return Object::isCursorInside();
	}

	void ImageButton::setPushedImageByName(chstr image)
	{
		setPushedImage(mDataset->getImage(image));
	}

	void ImageButton::setHoverImageByName(chstr image)
	{
		setHoverImage(mDataset->getImage(image));
	}

	void ImageButton::setDisabledImageByName(chstr image)
	{
		setDisabledImage(mDataset->getImage(image));
	}

	void ImageButton::setImage(Image* image)
	{
		ImageBox::setImage(image);
		mNormalImage = image;
	}
	
	bool ImageButton::onMouseDown(float x, float y, int button)
	{
		if (Object::onMouseDown(x, y, button))
		{
            triggerEvent("MouseDown", x, y, button);
			return true;
		}
		return ButtonBase::onMouseDown(x, y, button);
	}

	bool ImageButton::onMouseUp(float x, float y, int button)
	{
		if (Object::onMouseUp(x, y, button))
		{
			return true;
		}
		bool result = ButtonBase::onMouseUp(x, y, button);
		if (result)
		{
			triggerEvent("Click", x, y, button);
		}
		return result;
	}
	
	void ImageButton::onMouseMove(float x, float y)
	{
		Object::onMouseMove(x, y);
		ButtonBase::onMouseMove(x, y);
	}
	
	hstr ImageButton::getProperty(chstr name, bool* property_exists)
	{
		if (property_exists != NULL)
		{
			*property_exists = true;
		}
		if (name == "image")			return getImageName();
		if (name == "pushed_image")		return getPushedImage()->getName();
		if (name == "hover_image")		return getHoverImage()->getName();
		if (name == "disabled_image")	return getDisabledImage()->getName();
		return Object::getProperty(name, property_exists);
	}

	bool ImageButton::setProperty(chstr name, chstr value)
	{
		if      (name == "image")			setImageByName(value);
		else if (name == "pushed_image")	setPushedImageByName(value);
		else if (name == "hover_image")		setHoverImageByName(value);
		else if (name == "disabled_image")	setDisabledImageByName(value);
        else return Object::setProperty(name, value);
        return true;
	}
	
}

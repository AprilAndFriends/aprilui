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
		ImageBox(name, rect)
	{
		_setTypeName("ImageButton");
		mPushed = false;
		mNormalImage = NULL;
		mPushedImage = NULL;
		mHoverImage = NULL;
		mDisabledImage = NULL;
	}

	void ImageButton::OnDraw(gvec2 offset)
	{
		grect rect = mRect + offset;
		if (!isDerivedEnabled() && mDisabledImage != NULL)
		{
			mDisabledImage->draw(rect);
			return;
		}
		if (mPushed && mPushedImage == NULL && isCursorInside())
		{
			april::Color color;
			color *= 0.7f;
			color.a = getDerivedAlpha() * 255;
			mImage->draw(rect, color);
			return;
		}
		ImageBox::OnDraw(offset);
	}

	void ImageButton::update(float k)
	{
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
		else if (isCursorInside())
		{
			if (mPushed)
			{
				if (mPushedImage != NULL)
				{
					mImage = mPushedImage;
				}
			}
			else if (mHoverImage != NULL)
			{
				mImage = mHoverImage;
			}
		}
		ImageBox::update(k);
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
	
	bool ImageButton::OnMouseDown(float x, float y, int button)
	{
		if (Object::OnMouseDown(x, y, button))
		{
			return true;
		}
		if (isCursorInside())
		{
			mPushed = true;
			triggerEvent("MouseDown", x, y, 0);
			return true;
		}
		return false;
	}

	bool ImageButton::OnMouseUp(float x, float y, int button)
	{
		if (Object::OnMouseUp(x, y, button))
		{
			return true;
		}
		if (mPushed && isCursorInside())
		{
			mPushed = false;
			triggerEvent("Click", x, y, 0);
			return true;
		}
		mPushed = false;
		return false;
	}

	void ImageButton::OnMouseMove(float x, float y)
	{
		Object::OnMouseMove(x, y);
	}

	void ImageButton::setProperty(chstr name, chstr value)
	{
		Object::setProperty(name, value);
		if		(name == "image")			setImage(mDataset->getImage(value));
		else if (name == "pushed_image")	setPushedImage(mDataset->getImage(value));
		else if (name == "hover_image")		setHoverImage(mDataset->getImage(value));
		else if (name == "disabled_image")	setDisabledImage(mDataset->getImage(value));
	}
	
}

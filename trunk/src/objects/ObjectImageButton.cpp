/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com)                                  *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <hltypes/hstring.h>

#include "Dataset.h"
#include "Image.h"
#include "ObjectImageButton.h"

namespace AprilUI
{
	ImageButton::ImageButton(chstr name,float x,float y,float w,float h) :
		ImageBox(name,x,y,w,h)
	{
		_setTypeName("ImageButton");
		mPushed=0;
		mXMouse=0;
		mYMouse=0;
		mNormalImage=mPushedImage=mHoverImage=mDisabledImage=0;
	}

	void ImageButton::OnDraw(float offset_x,float offset_y)
	{
		float alpha=getDerivedAlpha();
		mImage=mNormalImage;
		if (!mImage)
		{
			mImage=mDataPtr->getImage("null");
		}
		if (!getDerivedEnabled())
		{
			if (!mDisabledImage)
			{
				alpha/=2;
				mImage->draw(mX+offset_x,mY+offset_y,mWidth,mHeight,1,1,1,alpha);
				return;
			}
			mImage=mDisabledImage;
		}
		else if (isPointInside(mXMouse,mYMouse))
		{
			if (mPushed && mPushedImage)
			{
				mImage=mPushedImage;
			}
			else if (mHoverImage)
			{
				mImage=mHoverImage;
			}
		}
		ImageBox::OnDraw(offset_x,offset_y);
	}

	void ImageButton::setPushedImage(Image* image)
	{
		mPushedImage=image;
	}

	void ImageButton::setHoverImage(Image* image)
	{
		mHoverImage=image;
	}

	void ImageButton::setDisabledImage(Image* image)
	{
		mDisabledImage=image;
	}

	void ImageButton::setPushedImageByName(chstr image)
	{
		setPushedImage(mDataPtr->getImage(image));
	}

	void ImageButton::setHoverImageByName(chstr image)
	{
		setHoverImage(mDataPtr->getImage(image));
	}

	void ImageButton::setDisabledImageByName(chstr image)
	{
		setDisabledImage(mDataPtr->getImage(image));
	}

	void ImageButton::setImage(Image* image)
	{
		mNormalImage=image;
	}
	
	void ImageButton::setImageByName(chstr image)
	{
		setImage(mDataPtr->getImage(image));
		mImageName=image;
	}
	
	void ImageButton::OnUpdate(float k)
	{
	}

	bool ImageButton::OnMouseDown(float x,float y,int button)
	{
		if (Object::OnMouseDown(x,y,button)) return true;
		if (isPointInside(mXMouse,mYMouse))
		{
			mPushed=true;
			return true;
		}
		return false;
	}

	bool ImageButton::OnMouseUp(float x,float y,int button)
	{
		if (Object::OnMouseUp(x,y,button)) return true;
		if (mPushed && isPointInside(mXMouse,mYMouse))
		{
			mPushed=false;
			triggerEvent("Click",x,y,0);
			return true;
		}
		mPushed=false;
		return false;
	}

	void ImageButton::OnMouseMove(float x,float y)
	{
		Object::OnMouseMove(x,y);
		mXMouse=x;
		mYMouse=y;
	}

	void ImageButton::setProperty(chstr name,chstr value)
	{
		Object::setProperty(name,value);
		if (name == "image")			setImage(mDataPtr->getImage(value));
		if (name == "pushed_image")     setPushedImage(mDataPtr->getImage(value));
		if (name == "hover_image")      setHoverImage(mDataPtr->getImage(value));
		if (name == "disabled_image")   setDisabledImage(mDataPtr->getImage(value));
	}
	
}

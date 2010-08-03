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
		mNormalImage=mPushedImage=mHoverImage=mDisabledImage=0;
	}

	void ImageButton::OnDraw(float offset_x,float offset_y)
	{
		float alpha=getDerivedAlpha();
		if (!getDerivedEnabled() && mDisabledImage)
		{
			alpha/=2;
			mDisabledImage->draw(mX+offset_x,mY+offset_y,mWidth,mHeight,1,1,1,alpha);
		}
		else if (mImage) ImageBox::OnDraw(offset_x,offset_y);
		else
		{
			Image* image=mNormalImage;
			if (!image) image=mDataPtr->getImage("null");
			image->draw(mX+offset_x,mY+offset_y,mWidth,mHeight,0.5f,0.5f,0.5f,alpha);
		}
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
		ImageBox::setImage(image);
		mNormalImage=mImage;
	}

	void ImageButton::OnUpdate(float k)
	{
	}

	bool ImageButton::OnMouseDown(int button,float x,float y)
	{
		if (Object::OnMouseDown(button,x,y)) return true;
		if (isPointInside(x,y))
		{
			mPushed=true;
			mImage=mPushedImage;
			return true;
		}
		return false;
	}

	bool ImageButton::OnMouseUp(int button,float x,float y)
	{
		if (Object::OnMouseUp(button,x,y)) return true;
		mImage=mNormalImage;
		if (mPushed && isPointInside(x,y))
		{
			mPushed=false;
			triggerEvent("Click",x,y,0);
			return true;
		}
		mPushed=false;
		return false;
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

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

#include "AprilUI.h"
#include "Dataset.h"
#include "Image.h"
#include "ImageBox.h"

namespace AprilUI
{
	ImageBox::ImageBox(chstr name,float x,float y,float w,float h) :
		Object("ImageBox",name,x,y,w,h)
	{
		mImage=0;
	}

	void ImageBox::setImage(Image* image)
	{
		mImage=image;
		if (image)
			mImageName=image->getName();
		else
			mImageName="null";
		if (mWidth == -1) mWidth=image->getSourceW()*getDefaultScale();
		if (mHeight == -1) mHeight=image->getSourceH()*getDefaultScale();
	}

	void ImageBox::setImageByName(chstr image)
	{
		setImage(mDataPtr->getImage(image));
		mImageName=image;
	}
	
	void ImageBox::notifyEvent(chstr event_name,void* params)
	{	
		if (event_name == "UpdateImage")
		{
			setImageByName(mImageName);
		}
		Object::notifyEvent(event_name,params);
	}
	
	void ImageBox::resizeToFitImage()
	{
		if (mImage) setSize(mImage->getSourceW()*getDefaultScale(), mImage->getSourceH()*getDefaultScale());
	}

	void ImageBox::OnDraw(float offset_x,float offset_y)
	{
		if (!mImage) mImage=mDataPtr->getImage("null");
		float alpha=getDerivedAlpha();
		if (!getDerivedEnabled()) alpha/=2;
		mImage->draw(mX+offset_x,mY+offset_y,mWidth,mHeight,1,1,1,alpha);
		//rendersys->setBlendMode(April::ALPHA_BLEND);
	}

	void ImageBox::setProperty(chstr name,chstr value)
	{
		Object::setProperty(name,value);
		if (name == "image") setImageByName(value);
	}

	bool ImageBox::OnMouseDown(int button,float x,float y)
	{
		if (Object::OnMouseDown(button,x,y)) return true;
		if (isPointInside(x,y))
		{
			return true;
		}
		return false;
	}

	bool ImageBox::OnMouseUp(int button,float x,float y)
	{
		if (Object::OnMouseUp(button,x,y)) return true;
		if (isPointInside(x,y))
		{
			triggerEvent("Click",x,y,0);
			return true;
		}
		return false;
	}
	
}

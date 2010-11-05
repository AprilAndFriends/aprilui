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

#include "AprilUI.h"
#include "Dataset.h"
#include "Image.h"
#include "ObjectColoredImageBox.h"

namespace AprilUI
{
	ImageBox::ImageBox(chstr name,grect rect) :
		Object("ImageBox",name,rect)
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
		grect rect = image->getSource();
		if (mRect.w == -1) mRect.w=rect.w*getDefaultScale();
		if (mRect.h == -1) mRect.h=rect.h*getDefaultScale();
	}

	void ImageBox::setImageByName(chstr image)
	{
		setImage(mDataset->getImage(image));
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
		if (mImage)
		{
			grect rect = mImage->getSource();
			setSize(rect.w*getDefaultScale(), rect.h*getDefaultScale());
		}
	}

	void ImageBox::OnDraw(float offset_x,float offset_y)
	{
		if (!mImage) mImage=mDataset->getImage("null");
		float alpha=getDerivedAlpha();
		if (!isDerivedEnabled()) alpha/=2;
		mImage->draw(mRect + gvec2(offset_x, offset_y),1,1,1,alpha);
		//rendersys->setBlendMode(April::ALPHA_BLEND);
	}

	void ImageBox::setProperty(chstr name,chstr value)
	{
		Object::setProperty(name,value);
		if (name == "image") setImageByName(value);
	}

	bool ImageBox::OnMouseDown(float x,float y,int button)
	{
		if (Object::OnMouseDown(x,y,button)) return true;
		if (isPointInside(x,y))
		{
			return true;
		}
		return false;
	}

	bool ImageBox::OnMouseUp(float x,float y,int button)
	{
		if (Object::OnMouseUp(x,y,button)) return true;
		if (isPointInside(x,y))
		{
			triggerEvent("Click",x,y,0);
			return true;
		}
		return false;
	}
	
}

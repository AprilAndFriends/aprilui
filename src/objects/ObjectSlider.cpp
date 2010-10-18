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

#include "Image.h"
#include "ObjectSlider.h"

namespace AprilUI
{
	Slider::Slider(chstr name,float x,float y,float w,float h) :
		ImageBox(name,x,y,w,h)
	{
		_setTypeName("Slider");
		mValue=0;
		mPushed=0;
	}

	bool Slider::OnMouseDown(float x,float y,int button)
	{
		if (Object::OnMouseDown(x,y,button)) return true;
		if (isPointInside(x,y))
		{
			mPushed=true;
			mValue=(x-mX)/mWidth;
			triggerEvent("Set",x,y,0);
			return true;
		}
		return false;
	}

	bool Slider::OnMouseUp(float x,float y,int button)
	{
		if (Object::OnMouseUp(x,y,button)) return true;
		if (mPushed && isPointInside(x,y))
		{
			mPushed=false;
			return true;
		}
		mPushed=false;
		return false;
	}

	void Slider::OnMouseMove(float x,float y)
	{
		Object::OnMouseMove(x,y);
		if (mPushed)
		{
			if (x < mX) x=mX; if (x > mX+mWidth) x=mX+mWidth;
			mValue=(x-mX)/mWidth;
			triggerEvent("Set",x,y,0);
		}
	}

	void Slider::OnDraw(float offset_x,float offset_y)
	{
		float x=mX+offset_x,y=mY+offset_y;
		float alpha=getDerivedAlpha();
		//rendersys->drawColoredQuad(x,y,mWidth,mHeight,1,1,1,alpha/2);
		April::rendersys->drawColoredQuad(x+mHeight/2,y+mHeight*0.375f,mWidth-mHeight,mHeight/4,0,0,0,alpha);
		April::rendersys->drawColoredQuad(x+mHeight/2+1,y+1+mHeight*0.375f,mWidth-2-mHeight,mHeight/4-2,0.89f,0.75f,0.49f,alpha);
		April::rendersys->drawColoredQuad(x+mHeight/2+2,y+2+mHeight*0.375f,mValue*(mWidth-mHeight-4),mHeight/4-4,0,0,0,alpha);
		mImage->draw(x+mHeight/4+mValue*(mWidth-mHeight),y+mHeight/4,mHeight/2,mHeight/2,1,1,1,alpha);
	}

	void Slider::setProperty(chstr name,chstr value)
	{
		ImageBox::setProperty(name,value);
	}
	
}

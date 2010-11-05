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

#include "Image.h"
#include "ObjectSlider.h"

namespace AprilUI
{
	Slider::Slider(chstr name,grect rect) :
		ImageBox(name,rect)
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
			mValue=(x-mRect.x)/mRect.w;
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
			if (x < mRect.x) x=mRect.x; if (x > mRect.x+mRect.w) x=mRect.x+mRect.h;
			mValue=(x-mRect.x)/mRect.w;
			triggerEvent("Set",x,y,0);
		}
	}

	void Slider::OnDraw(float offset_x,float offset_y)
	{
		float x=mRect.x+offset_x,y=mRect.y+offset_y;
		float alpha=getDerivedAlpha();
		April::rendersys->drawColoredQuad(x+mRect.h/2,y+mRect.h*0.375f,mRect.w-mRect.h,mRect.h/4,0,0,0,alpha);
		April::rendersys->drawColoredQuad(x+mRect.h/2+1,y+1+mRect.h*0.375f,mRect.w-2-mRect.h,mRect.h/4-2,0.89f,0.75f,0.49f,alpha);
		April::rendersys->drawColoredQuad(x+mRect.h/2+2,y+2+mRect.h*0.375f,mValue*(mRect.w-mRect.h-4),mRect.h/4-4,0,0,0,alpha);
		grect rect = mRect / 2;
		rect.x = x+mRect.h/4+mValue*(mRect.w-mRect.h);
		rect.y = y+mRect.h/4;
		mImage->draw(rect,1,1,1,alpha);
	}

	void Slider::setProperty(chstr name,chstr value)
	{
		ImageBox::setProperty(name,value);
	}
	
}

/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <hltypes/hstring.h>

#include "Event.h"
#include "EventArgs.h"
#include "Image.h"
#include "ToggleButton.h"

namespace AprilUI
{
	ToggleButton::ToggleButton(chstr name,float x,float y,float w,float h) :
		ImageButton(name,x,y,w,h)
	{
		_setTypeName("ToggleButton");
		mPushed=0;
	}

	void ToggleButton::OnDraw(float offset_x,float offset_y)
	{
		float alpha=getDerivedAlpha();
		if (mPushed) mPushedImage->draw(mX+offset_x,mY+offset_y,mWidth,mHeight,1,1,1,alpha);
		mImage->draw(mX+offset_x,mY+offset_y,mWidth,mHeight,1,1,1,alpha);
	}

	bool ToggleButton::OnMouseDown(int button,float x,float y)
	{
		if (Object::OnMouseDown(button,x,y)) return true;
		if (isPointInside(x,y))
		{
			mPushed=!mPushed;
			Event* e;
			if (mPushed) e=mEvents["Toggle"];
			else         e=mEvents["Untoggle"];
			if (e)
			{
				EventArgs args(this,x,y);
				e->execute(&args);
			}
			return true;
		}
		return false;
	}

	bool ToggleButton::OnMouseUp(int button,float x,float y)
	{
		if (Object::OnMouseUp(button,x,y)) return true;
		if (isPointInside(x,y))
		{
			triggerEvent("Click",x,y,0);
			return 1;
		}
		return 0;
	}
	
}

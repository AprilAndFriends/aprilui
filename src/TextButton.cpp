/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <atres/Atres.h>
#include <hltypes/hstring.h>

#include "Dataset.h"
#include "Objects.h"

namespace AprilUI
{
	TextButton::TextButton(chstr name,float x,float y,float w,float h) :
		Label(name,x,y,w,h)
	{
		mHorzFormatting=Atres::CENTER;
		mVertFormatting=VERT_CENTER;
		mFontEffect=Atres::NONE;
		mText="TextButton: "+name;
		mTypeName="TextButton";
		mPushed=0;
		mBackgroundEnabled=1;
	}

	void TextButton::setTextKey(chstr key)
	{
		setText(mDataPtr->getText(key));
	}

	void TextButton::OnDraw(float offset_x,float offset_y)
	{
		unsigned char a=mTextColor.a;
		if (mBackgroundEnabled)
			rendersys->drawColoredQuad(mX+offset_x, mY+offset_y, mWidth, mHeight, 0, 0, 0, 0.7f+0.3f*mPushed);
		else
			mTextColor.a*=(unsigned char)(1.0f-0.3f*mPushed);
		Label::OnDraw(offset_x,offset_y);
		if (!mBackgroundEnabled) mTextColor.a=a;
	}
	
	void TextButton::setProperty(chstr name,chstr value)
	{
		if (name == "background") mBackgroundEnabled=((int) value)!=0;
		else Label::setProperty(name,value);
	}

	bool TextButton::OnMouseDown(int button,float x,float y)
	{
		if (Object::OnMouseDown(button,x,y)) return true;
		if (isPointInside(x,y))
		{
			mPushed=true;
			return true;
		}
		return false;
	}

	bool TextButton::OnMouseUp(int button,float x,float y)
	{
		if (Object::OnMouseUp(button,x,y)) return true;
		if (mPushed && isPointInside(x,y))
		{
			mPushed=false;
			triggerEvent("Click",x,y,0);
			return true;
		}
		mPushed=false;
		return false;
	}
	
}

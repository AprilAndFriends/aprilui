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

#include "AprilUI.h"
#include "Dataset.h"
#include "ObjectTextButton.h"

namespace AprilUI
{
	TextButton::TextButton(chstr name,float x,float y,float w,float h) :
		Label(name,x,y,w,h)
	{
		mText="TextButton: "+name;
		mTypeName="TextButton";
		mPushed=0;
		mBackgroundEnabled=1;
		mPushedTextColor.setColor("FF333333");
		mHoverTextColor.setColor("FF7F7F7F");
	}

	void TextButton::setTextKey(chstr key)
	{
		setText(mDataPtr->getText(key));
	}

	void TextButton::OnDraw(float offset_x,float offset_y)
	{
		April::Color temp=mTextColor;
#ifndef _DEBUG
		if (mBackgroundEnabled)
#else
		if (!AprilUI::isDebugMode() && mBackgroundEnabled)
#endif
			April::rendersys->drawColoredQuad(mX+offset_x, mY+offset_y, mWidth, mHeight, 0, 0, 0, 0.7f+0.3f*mPushed);
		if (mPushed) mTextColor=mPushedTextColor;
		else if (isCursorInside()) mTextColor=mHoverTextColor;
		Label::OnDraw(offset_x,offset_y);

		mTextColor=temp;
	}
	
	void TextButton::setHoverTextColor(April::Color color)
	{
		mHoverTextColor=color;
	}

	void TextButton::setHoverTextColor(chstr hex)
	{
		mHoverTextColor.setColor(hex);
	}

	void TextButton::setPushedTextColor(April::Color color)
	{
		mPushedTextColor=color;
	}

	void TextButton::setPushedTextColor(chstr hex)
	{
		mHoverTextColor.setColor(hex);
	}
	
	void TextButton::setProperty(chstr name,chstr value)
	{
		if (name == "background") mBackgroundEnabled=((int) value)!=0;
		else if (name == "hover_color") setHoverTextColor(value);
		else if (name == "pushed_color") setPushedTextColor(value);
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

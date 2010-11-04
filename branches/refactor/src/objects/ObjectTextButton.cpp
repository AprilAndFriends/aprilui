/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
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
	TextButton::TextButton(chstr name,grect rect) :
		Label(name,rect)
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
#ifndef _DEBUG
		if (mBackgroundEnabled)
#else
		if (!AprilUI::isDebugMode() && mBackgroundEnabled)
#endif
			April::rendersys->drawColoredQuad(mX+offset_x, mY+offset_y, mWidth, mHeight, 0, 0, 0, 0.7f+0.3f*mPushed);
		April::Color color=mTextColor;
		if (isCursorInside())
		{
			mTextColor=(mPushed ? mPushedTextColor : mHoverTextColor);
		}
		Label::OnDraw(offset_x,offset_y);
		mTextColor=color;
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
		mPushedTextColor.setColor(hex);
	}
	
	void TextButton::setProperty(chstr name,chstr value)
	{
		if (name == "background") mBackgroundEnabled=((int) value)!=0;
		else if (name == "hover_color") setHoverTextColor(value);
		else if (name == "pushed_color") setPushedTextColor(value);
		else Label::setProperty(name,value);
	}

	bool TextButton::OnMouseDown(float x,float y,int button)
	{
		if (Object::OnMouseDown(x,y,button)) return true;
		if (isCursorInside())
		{
			mPushed=true;
			return true;
		}
		return false;
	}

	bool TextButton::OnMouseUp(float x,float y,int button)
	{
		if (Object::OnMouseUp(x,y,button)) return true;
		if (mPushed && isCursorInside())
		{
			mPushed=false;
			triggerEvent("Click",x,y,0);
			return true;
		}
		mPushed=false;
		return false;
	}
	
}

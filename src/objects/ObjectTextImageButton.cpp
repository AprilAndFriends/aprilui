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

#include "Dataset.h"
#include "ObjectTextImageButton.h"

namespace AprilUI
{
	TextImageButton::TextImageButton(chstr name,float x,float y,float w,float h) :
		LabelBase(name),
		ImageButton(name,x,y,w,h)
	{
		_setTypeName("TextImageButton");
		mText="TextImageButton: "+name;
	}

	void TextImageButton::OnDraw(float offset_x,float offset_y)
	{
		ImageButton::OnDraw(offset_x,offset_y);
		float alpha=getDerivedAlpha();
		if (!getDerivedEnabled() || !mImage && !mNormalImage && !mPushedImage && mPushed) alpha/=2;
		LabelBase::_drawLabel(mX+offset_x,mY+offset_y,mWidth,mHeight,alpha);
	}

	void TextImageButton::setTextKey(chstr key)
	{
		setText(mDataPtr->getText(key));
	}

	void TextImageButton::setProperty(chstr name,chstr value)
	{
		LabelBase::setProperty(name,value);
		ImageButton::setProperty(name,value);
		if (name == "textkey") setTextKey(value);
	}
	
}

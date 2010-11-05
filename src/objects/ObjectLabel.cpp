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

#include "Dataset.h"
#include "ObjectLabel.h"

namespace AprilUI
{
	Label::Label(chstr name,grect rect) :
		LabelBase(name),
		Object("Label",name,rect)
	{
	}

	void Label::OnDraw(float offset_x,float offset_y)
	{
		Object::OnDraw(offset_x, offset_y);
		float alpha=getDerivedAlpha();
		if (!isDerivedEnabled()) alpha/=2;
		LabelBase::_drawLabel(mRect.x+offset_x,mRect.y+offset_y,mRect.w,mRect.h,alpha);
	}

	void Label::notifyEvent(chstr event_name,void* params)
	{
		if (event_name == "UpdateText")
			setTextKey(mTextKey);
		Object::notifyEvent(event_name,params);
	}

	void Label::setTextKey(chstr key)
	{
		mTextKey=key;
		setText(mDataset->getText(key));
	}

	void Label::setProperty(chstr name,chstr value)
	{
		LabelBase::setProperty(name,value);
		Object::setProperty(name,value);
		if (name == "textkey") setTextKey(value);
	}
	
}

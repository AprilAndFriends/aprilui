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
#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>

#include "Dataset.h"
#include "ObjectLabel.h"

namespace aprilui
{
	Label::Label(chstr name, grect rect) :
		LabelBase(),
		Object("Label", name, rect)
	{
		mText = "Label: " + name;
	}

	void Label::OnDraw(gvec2 offset)
	{
		Object::OnDraw(offset);
		float alpha = getDerivedAlpha();
		if (!isDerivedEnabled())
		{
			alpha /= 2;
		}
		LabelBase::_drawLabel(mRect + offset, alpha);
	}

	void Label::notifyEvent(chstr name, void* params)
	{
		if (name == "UpdateText")
		{
			setTextKey(mTextKey);
		}
		Object::notifyEvent(name, params);
	}

	void Label::setTextKey(chstr key)
	{
		mTextKey = key;
		setText(mDataset->getText(key));
	}

	void Label::setProperty(chstr name, chstr value)
	{
		LabelBase::setProperty(name, value);
		Object::setProperty(name, value);
		if (name == "textkey")	setTextKey(value);
	}
	
}

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
#include "ObjectTextImageButton.h"

namespace AprilUI
{
	TextImageButton::TextImageButton(chstr name, grect rect) :
		LabelBase(name),
		ImageButton(name, rect)
	{
		_setTypeName("TextImageButton");
		mText = "TextImageButton: " + name;
	}

	void TextImageButton::OnDraw(float offset_x, float offset_y)
	{
		ImageButton::OnDraw(offset_x, offset_y);
		float alpha = getDerivedAlpha();
		if (!isDerivedEnabled() || mImage == NULL && mNormalImage == NULL && mPushedImage == NULL && mPushed)
		{
			alpha /= 2;
		}
		LabelBase::_drawLabel(mRect.x + gvec2(offset_x, offset_y), alpha);
	}

	void TextImageButton::setTextKey(chstr key)
	{
		setText(mDataset->getText(key));
	}

	void TextImageButton::setProperty(chstr name,chstr value)
	{
		LabelBase::setProperty(name, value);
		ImageButton::setProperty(name, value);
		if (name == "textkey")	setTextKey(value);
	}
	
}

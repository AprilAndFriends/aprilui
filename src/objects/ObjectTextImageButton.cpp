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
#include "ObjectLabelBase.h"
#include "ObjectTextImageButton.h"

namespace AprilUI
{
	TextImageButton::TextImageButton(chstr name, grect rect) :
		LabelBase(),
		ImageButton(name, rect)
	{
		_setTypeName("TextImageButton");
		mText = "TextImageButton: " + name;
	}

	void TextImageButton::OnDraw(gvec2 offset)
	{
		ImageButton::OnDraw(offset);
		float alpha = getDerivedAlpha();
		if (!isDerivedEnabled() || mImage == NULL && mNormalImage == NULL && mPushedImage == NULL && mPushed)
		{
			alpha /= 2;
		}
		LabelBase::_drawLabel(mRect + offset, alpha);
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

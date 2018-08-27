/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>

#include "Dataset.h"
#include "ObjectTextImageButton.h"

namespace aprilui
{
	TextImageButton::TextImageButton(chstr name, grectf rect) :
		ImageButton(name, rect),
		LabelBase(name)
	{
		_setTypeName("TextImageButton");
		mText = "TextImageButton: " + name;
	}

	Dataset* TextImageButton::getDataset()
	{
		return ImageButton::getDataset();
	}

	void TextImageButton::OnDraw(gvec2f offset)
	{
		ImageButton::OnDraw(offset);
		float alpha = getDerivedAlpha();
		if (!isDerivedEnabled() || (mImage == NULL && mNormalImage == NULL && mPushedImage == NULL && mPushed))
		{
			alpha /= 2;
		}
		LabelBase::_drawLabel(_getDrawRect() + offset, alpha);
	}

	void TextImageButton::notifyEvent(chstr name, void* params)
	{
		ImageButton::notifyEvent(name, params);
		LabelBase::notifyEvent(name, params);
	}
	
	bool TextImageButton::setProperty(chstr name,chstr value)
	{
		LabelBase::setProperty(name, value);
		ImageButton::setProperty(name, value);
		if (name == "textkey")	setTextKey(value);
		return 1;
	}
	
}

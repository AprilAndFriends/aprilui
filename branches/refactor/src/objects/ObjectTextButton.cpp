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
	TextButton::TextButton(chstr name, grect rect) :
		Label(name, rect)
	{
		mText = "TextButton: " + name;
		mTypeName = "TextButton";
		mPushed = false;
		mBackground = true;
		mPushedTextColor.setColor("FF333333");
		mHoverTextColor.setColor("FF7F7F7F");
		mDisabledTextColor.setColor("FF7F7F7F");
	}

	void TextButton::setTextKey(chstr key)
	{
		setText(mDataset->getText(key));
	}

	void TextButton::OnDraw(float offset_x, float offset_y)
	{
		bool cursorInside = isCursorInside();
#ifndef _DEBUG
		if (mBackground)
#else
		if (!AprilUI::isDebugMode() && mBackground)
#endif
			April::rendersys->drawColoredQuad(mRect.x + offset_x, mRect.y + offset_y, mRect.w, mRect.h, 0, 0, 0, 0.7f + 0.3f * (cursorInside && mPushed));
		April::Color color = mTextColor;
		if (!isDerivedEnabled())
		{
			mTextColor = mDisabledTextColor;
		}
		else if (cursorInside)
		{
			mTextColor = (mPushed ? mPushedTextColor : mHoverTextColor);
		}
		Label::OnDraw(offset_x, offset_y);
		mTextColor = color;
	}
	
	void TextButton::setProperty(chstr name, chstr value)
	{
		Label::setProperty(name, value);
		if		(name == "background")		mBackground = (bool)value;
		else if (name == "hover_color")		setHoverTextColor(value);
		else if (name == "pushed_color")	setPushedTextColor(value);
		else if (name == "disabled_color")	setDisabledTextColor(value);
	}

	bool TextButton::OnMouseDown(float x, float y, int button)
	{
		if (Object::OnMouseDown(x, y, button))
		{
			return true;
		}
		if (isCursorInside())
		{
			mPushed = true;
			return true;
		}
		return false;
	}

	bool TextButton::OnMouseUp(float x, float y, int button)
	{
		if (Object::OnMouseUp(x, y, button))
		{
			return true;
		}
		if (mPushed && isCursorInside())
		{
			mPushed = false;
			triggerEvent("Click", x, y, 0);
			return true;
		}
		mPushed = false;
		return false;
	}
	
}

/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <atres/atres.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "Dataset.h"
#include "ObjectTextButton.h"

namespace aprilui
{
	TextButton::TextButton(chstr name, grect rect) :
		Label(name, rect)
	{
		mText = "TextButton: " + name;
		mTypeName = "TextButton";
		mPushed = false;
		mBackground = true;
		mPushedTextColor.set(0x33,0x33,0x33);
		mHoverTextColor.set(0x7F,0x7F,0x7F);
		mDisabledTextColor.set(0x7F,0x7F,0x7F);
	}

	void TextButton::OnDraw(gvec2 offset)
	{
		bool cursorInside = isCursorInside();
		if (mBackground)
		{
			grect rect = _getDrawRect() + offset;
			float a=0.5f + /* 0.25f * cursorInside + */ 0.25f * mPushed;
			april::rendersys->drawFilledRect(rect, april::Color(COLOR_COMP_FOR_NEW_APRIL(0), 
																 COLOR_COMP_FOR_NEW_APRIL(0),
																 COLOR_COMP_FOR_NEW_APRIL(0),
																 COLOR_COMP_FOR_NEW_APRIL(a)));
		}
#ifdef _DEBUG
		else if (aprilui::isDebugMode())
		{
			grect rect = _getDrawRect() + offset;
			april::rendersys->drawFilledRect(rect, april::Color(0, 0, 0, (int) (178 + 77 * (cursorInside && mPushed))));
		}
#endif
		april::Color color = mTextColor;
		if (!isDerivedEnabled())
		{
			mTextColor = mDisabledTextColor;
		}
		else if (cursorInside)
		{
			mTextColor = (mPushed ? mPushedTextColor : mHoverTextColor);
		}
		Label::OnDraw(offset);
		mTextColor = color;
	}
	
	bool TextButton::setProperty(chstr name, chstr value)
	{
		Label::setProperty(name, value);
		if		(name == "background")		mBackground = (bool)value;
		else if (name == "hover_color")		setHoverTextColor(value);
		else if (name == "pushed_color")	setPushedTextColor(value);
		else if (name == "disabled_color")	setDisabledTextColor(value);
		return 1;
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

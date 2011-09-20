/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <atres/atres.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "Dataset.h"
#include "ObjectTextButton.h"

namespace aprilui
{
	TextButton::TextButton(chstr name, grect rect) :
		ButtonBase(),
		Label(name, rect)
	{
		mText = "TextButton: " + name;
		mTypeName = "TextButton";
		mBackground = true;
		mPushedTextColor.set(51, 51, 51);
		mHoverTextColor.set(127, 127, 127);
		mDisabledTextColor.set(127, 127, 127);
	}

	void TextButton::setTextKey(chstr key)
	{
		setText(mDataset->getText(key));
	}
	
	void TextButton::update(float k)
	{
		Object::update(k);
		ButtonBase::update(k);
	}

	void TextButton::OnDraw()
	{
		if (mBackground)
		{
			april::rendersys->drawColoredQuad(_getDrawRect(), april::Color(0, 0, 0, ((mHover && mPushed) ? 255 : 191)));
		}
		april::Color color = mTextColor;
		if (!isDerivedEnabled())
		{
			mTextColor = mDisabledTextColor;
		}
		else if (mHover)
		{
			if (mPushed)
			{
				mTextColor = mPushedTextColor;
			}
			else if (aprilui::isHoverEffectEnabled())
			{
				mTextColor = mHoverTextColor;
			}
		}
		Label::OnDraw();
		mTextColor = color;
	}
	
	hstr TextButton::getProperty(chstr name, bool* property_exists)
	{
		if (name == "background")		return mBackground;
		if (name == "hover_color")		return getHoverTextColor().hex();
		if (name == "pushed_color")		return getPushedTextColor().hex();
		if (name == "disabled_color")	return getDisabledTextColor().hex();
		return Label::getProperty(name, property_exists);
	}
	
	bool TextButton::setProperty(chstr name, chstr value)
	{
		if		(name == "background")		mBackground = (bool)value;
		else if (name == "hover_color")		setHoverTextColor(value);
		else if (name == "pushed_color")	setPushedTextColor(value);
		else if (name == "disabled_color")	setDisabledTextColor(value);
		else return Label::setProperty(name, value);
		return true;
	}
	
	bool TextButton::isCursorInside()
	{
		return Object::isCursorInside();
	}

	bool TextButton::onMouseDown(float x, float y, int button)
	{
		if (Object::onMouseDown(x, y, button))
		{
			triggerEvent("MouseDown", x, y, button);
			return true;
		}
		return ButtonBase::onMouseDown(x, y, button);
	}

	bool TextButton::onMouseUp(float x, float y, int button)
	{
		if (Object::onMouseUp(x, y, button))
		{
			return true;
		}
		bool result = ButtonBase::onMouseUp(x, y, button);
		if (result)
		{
			triggerEvent("Click", x, y, button);
		}
		return result;
	}

	void TextButton::onMouseMove(float x, float y)
	{
		Object::onMouseMove(x, y);
		ButtonBase::onMouseMove(x, y);
	}

}

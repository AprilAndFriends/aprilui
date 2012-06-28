/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.7
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <atres/atres.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "Dataset.h"
#include "ObjectTextButton.h"

namespace aprilui
{
	TextButton::TextButton(chstr name, grect rect) :
		Label(name, rect),
		ButtonBase()
	{
		mText = "TextButton: " + name;
		mBackground = true;
		mPushedTextColor = APRIL_COLOR_WHITE / 5.0f;
		mHoverTextColor = APRIL_COLOR_GREY;
		mDisabledTextColor = APRIL_COLOR_GREY;
	}

	TextButton::~TextButton()
	{
	}

	Object* TextButton::createInstance(chstr name, grect rect)
	{
		return new TextButton(name, rect);
	}

	hstr TextButton::getName()
	{
		return Label::getName();
	}

	Object* TextButton::getParent()
	{
		return Label::getParent();
	}

	Dataset* TextButton::getDataset()
	{
		return Label::getDataset();
	}

	void TextButton::update(float k)
	{
		Object::update(k);
		ButtonBase::update(k);
	}

	void TextButton::OnDraw()
	{
		april::Color color = mTextColor;
		if (!_isDerivedEnabled())
		{
			mTextColor = mDisabledTextColor;
		}
		else if (mHovered)
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
		if (mBackground)
		{
			grect rect = _getDrawRect();
			april::Color drawColor = april::Color(APRIL_COLOR_BLACK, ((mHovered && mPushed) ? 255 : 191));
			april::rendersys->drawFilledRect(rect, drawColor);
			drawColor = april::Color(mTextColor, drawColor.a);
			april::rendersys->drawRect(rect, drawColor);
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

	bool TextButton::onMouseDown(int button)
	{
		if (Object::onMouseDown(button))
		{
			return true;
		}
		bool result = ButtonBase::onMouseDown(button);
		if (result)
		{
			_triggerEvent("MouseDown", button);
		}
		return result;
	}

	bool TextButton::onMouseUp(int button)
	{
		if (Object::onMouseUp(button))
		{
			return true;
		}
		bool result = ButtonBase::onMouseUp(button);
		if (result)
		{
			_triggerEvent("Click", button);
		}
		return result;
	}

	void TextButton::onMouseMove()
	{
		Object::onMouseMove();
		ButtonBase::onMouseMove();
	}

	void TextButton::cancelMouseDown()
	{
		Object::cancelMouseDown();
		ButtonBase::cancelMouseDown();
	}

}
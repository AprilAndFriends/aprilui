/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.4
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
            april::rendersys->drawColoredQuad(rect, drawColor);
			drawColor = april::Color(mTextColor, drawColor.a);
            april::rendersys->drawQuad(rect, drawColor);
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
			return true;
		}
		bool result = ButtonBase::onMouseDown(x, y, button);
		if (result)
		{
			_triggerEvent("MouseDown", x, y, button);
		}
		return result;
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
			_triggerEvent("Click", x, y, button);
		}
		return result;
	}

	void TextButton::onMouseMove(float x, float y)
	{
		Object::onMouseMove(x, y);
		ButtonBase::onMouseMove(x, y);
	}

	void TextButton::cancelMouseDown()
	{
		Object::cancelMouseDown();
		ButtonBase::cancelMouseDown();
	}

}

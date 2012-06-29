/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.75
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "Dataset.h"
#include "ObjectLabelBase.h"
#include "ObjectTextImageButton.h"

namespace aprilui
{
	TextImageButton::TextImageButton(chstr name, grect rect) :
		LabelBase(),
		ImageButton(name, rect)
	{
		mText = "TextImageButton: " + name;
		mUseBackground = false;
		mPushedTextColor = APRIL_COLOR_WHITE / 5.0f;
		mHoverTextColor = APRIL_COLOR_GREY;
		mDisabledTextColor = APRIL_COLOR_GREY;
		_mUseHoverTextColor = false;
		_mUsePushedTextColor = false;
		_mUseDisabledTextColor = false;
	}

	TextImageButton::~TextImageButton()
	{
	}

	Object* TextImageButton::createInstance(chstr name, grect rect)
	{
		return new TextImageButton(name, rect);
	}

	Dataset* TextImageButton::getDataset()
	{
		return ImageButton::getDataset();
	}

	void TextImageButton::OnDraw()
	{
		ImageButton::OnDraw();
		april::Color color = mTextColor;
		april::Color drawColor = _getDrawColor();
		if (!_isDerivedEnabled())
		{
			if (_mUseDisabledTextColor)
			{
				mTextColor = mDisabledTextColor;
			}
			else
			{
				drawColor.a = (unsigned char)(drawColor.a * _getDisabledAlphaFactor());
			}
		}
		else if (mHovered)
		{
			if (mPushed)
			{
				if (_mUsePushedTextColor)
				{
					mTextColor = mPushedTextColor;
				}
			}
			else if (aprilui::isHoverEffectEnabled() && _mUseHoverTextColor)
			{
				mTextColor = mHoverTextColor;
			}
		}
		grect rect = _getDrawRect();
		if (mUseBackground)
		{
			april::Color backgroundColor = april::Color(APRIL_COLOR_BLACK, (unsigned char)(((mHovered && mPushed) ? 255 : 191) * drawColor.a_f()));
			april::rendersys->drawFilledRect(rect, backgroundColor);
			backgroundColor = april::Color(mTextColor, backgroundColor.a);
			april::rendersys->drawRect(rect, backgroundColor);
		}
		LabelBase::_drawLabel(rect, drawColor);
		mTextColor = color;
	}
	
	void TextImageButton::notifyEvent(chstr name, void* params)
	{
		ImageButton::notifyEvent(name, params);
		LabelBase::notifyEvent(name, params);
	}
	
	hstr TextImageButton::getProperty(chstr name, bool* property_exists)
	{
		bool exists = false;
		hstr result = LabelBase::getProperty(name, &exists);
		if (!exists)
		{
			return ImageButton::getProperty(name, property_exists);
		}
		if (property_exists != NULL)
		{
			*property_exists = exists;
		}
		if (name == "use_disabled_color")
		{
			aprilui::log("WARNING: 'use_disabled_color' is deprecated, use 'disabled_text_color' instead!"); // DEPRECATED
			return _mUseDisabledTextColor;
		}
		if (name == "hover_text_color")		return getHoverTextColor().hex();
		if (name == "pushed_text_color")	return getPushedTextColor().hex();
		if (name == "disabled_text_color")	return getDisabledTextColor().hex();
		return result;
	}
	
	bool TextImageButton::setProperty(chstr name, chstr value)
	{
		if (name == "use_disabled_color")
		{
			aprilui::log("WARNING: 'use_disabled_color=' is deprecated, use 'disabled_text_color=' instead!"); // DEPRECATED
			_mUseDisabledTextColor = !value;
			setDisabledTextColor(mTextColor);
		}
		else if (name == "hover_text_color")	setHoverTextColor(value);
		else if (name == "pushed_text_color")	setPushedTextColor(value);
		else if (name == "disabled_text_color")	setDisabledTextColor(value);
		else if (LabelBase::setProperty(name, value)) { }
		else return ImageButton::setProperty(name, value);
		return true;
	}
	
}

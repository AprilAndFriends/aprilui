/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.91
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
		this->mText = "TextImageButton: " + name;
		this->mUseBackground = false;
		this->mPushedTextColor = APRIL_COLOR_WHITE / 5.0f;
		this->mHoverTextColor = APRIL_COLOR_GREY;
		this->mDisabledTextColor = APRIL_COLOR_GREY;
		this->_mUseHoverTextColor = false;
		this->_mUsePushedTextColor = false;
		this->_mUseDisabledTextColor = false;
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
		april::Color color = this->mTextColor;
		april::Color drawColor = this->_getDrawColor();
		if (!this->_isDerivedEnabled())
		{
			if (this->_mUseDisabledTextColor)
			{
				this->mTextColor = this->mDisabledTextColor;
			}
			else
			{
				drawColor.a = (unsigned char)(drawColor.a * this->_getDisabledAlphaFactor());
			}
		}
		else if (this->mHovered)
		{
			if (this->mPushed)
			{
				if (this->_mUsePushedTextColor)
				{
					this->mTextColor = this->mPushedTextColor;
				}
			}
			else if (aprilui::isHoverEffectEnabled() && this->_mUseHoverTextColor)
			{
				this->mTextColor = this->mHoverTextColor;
			}
		}
		grect rect = this->_getDrawRect();
		if (this->mUseBackground)
		{
			april::Color backgroundColor = april::Color(APRIL_COLOR_BLACK, (unsigned char)(((this->mHovered && this->mPushed) ? 255 : 191) * drawColor.a_f()));
			april::rendersys->drawFilledRect(rect, backgroundColor);
			backgroundColor = april::Color(this->mTextColor, backgroundColor.a);
			april::rendersys->drawRect(rect, backgroundColor);
		}
		LabelBase::_drawLabel(rect, drawColor);
		this->mTextColor = color;
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
			return this->_mUseDisabledTextColor;
		}
		if (name == "use_background")		return this->isUseBackground();
		if (name == "hover_text_color")		return this->getHoverTextColor().hex();
		if (name == "pushed_text_color")	return this->getPushedTextColor().hex();
		if (name == "disabled_text_color")	return this->getDisabledTextColor().hex();
		return result;
	}
	
	bool TextImageButton::setProperty(chstr name, chstr value)
	{
		if (name == "use_disabled_color")
		{
			aprilui::log("WARNING: 'use_disabled_color=' is deprecated, use 'disabled_text_color=' instead!"); // DEPRECATED
			this->_mUseDisabledTextColor = !value;
			this->setDisabledTextColor(this->mTextColor);
		}
		else if (name == "use_background")		this->setUseBackground(value);
		else if (name == "hover_text_color")	this->setHoverTextColor(value);
		else if (name == "pushed_text_color")	this->setPushedTextColor(value);
		else if (name == "disabled_text_color")	this->setDisabledTextColor(value);
		else if (LabelBase::setProperty(name, value)) { }
		else return ImageButton::setProperty(name, value);
		return true;
	}
	
}

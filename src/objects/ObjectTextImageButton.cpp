/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.52
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hlog.h>
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
		this->mPushedTextColor = april::Color::White * 0.2f;
		this->mHoverTextColor = april::Color::Grey;
		this->mDisabledTextColor = april::Color::Grey;
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
		unsigned char alpha = this->mBackgroundColor.a;
		if (!this->isDerivedEnabled())
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
			this->mBackgroundColor.a = (unsigned char)(this->mBackgroundColor.a * 0.75f);
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
		this->mBackgroundColor.a = (unsigned char)(this->mBackgroundColor.a * this->_getDisabledAlphaFactor());
		LabelBase::_drawLabel(rect, drawColor, this->mBackgroundColor);
		this->mTextColor = color;
		this->mBackgroundColor.a = alpha;
	}
	
	void TextImageButton::notifyEvent(chstr name, void* params)
	{
		LabelBase::notifyEvent(name, params);
		ImageButton::notifyEvent(name, params);
	}
	
	bool TextImageButton::triggerEvent(chstr name, april::Key keyCode, chstr extra)
	{
		return ImageButton::triggerEvent(name, keyCode, extra);
	}

	bool TextImageButton::triggerEvent(chstr name, float x, float y, april::Key keyCode, chstr extra)
	{
		return ImageButton::triggerEvent(name, x, y, keyCode, extra);
	}

	hstr TextImageButton::getProperty(chstr name, bool* propertyExists)
	{
		if (propertyExists != NULL)
		{
			*propertyExists = true;
		}
		if (name == "use_disabled_color")
		{
			hlog::warn(aprilui::logTag, "'use_disabled_color' is deprecated, use 'disabled_text_color' instead!"); // DEPRECATED
			return this->_mUseDisabledTextColor;
		}
		if (name == "hover_text_color")		return this->getHoverTextColor().hex();
		if (name == "pushed_text_color")	return this->getPushedTextColor().hex();
		if (name == "disabled_text_color")	return this->getDisabledTextColor().hex();
		bool exists = false;
		hstr result = LabelBase::getProperty(name, &exists);
		if (!exists)
		{
			result = ImageButton::getProperty(name, &exists);
		}
		if (propertyExists != NULL)
		{
			*propertyExists = exists;
		}
		return result;
	}
	
	bool TextImageButton::setProperty(chstr name, chstr value)
	{
		if (name == "use_disabled_color")
		{
			hlog::warn(aprilui::logTag, "'use_disabled_color=' is deprecated, use 'disabled_text_color=' instead!"); // DEPRECATED
			this->_mUseDisabledTextColor = !value;
			this->setDisabledTextColor(this->mTextColor);
		}
		else if (name == "hover_text_color")	this->setHoverTextColor(value);
		else if (name == "pushed_text_color")	this->setPushedTextColor(value);
		else if (name == "disabled_text_color")	this->setDisabledTextColor(value);
		else if (LabelBase::setProperty(name, value)) { }
		else return ImageButton::setProperty(name, value);
		return true;
	}
	
}

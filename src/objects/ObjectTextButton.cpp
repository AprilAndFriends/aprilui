/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <atres/atres.h>
#include <hltypes/hlog.h>
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
		hlog::warn(aprilui::logTag, "'TextButton' is deprecated, use 'TextImageButton' without images instead!"); // DEPRECATED
		this->mText = "TextButton: " + name;
		this->mBackgroundColor = april::Color::Black;
		this->mPushedTextColor = april::Color::White / 5.0f;
		this->mHoverTextColor = april::Color::Grey;
		this->mDisabledTextColor = april::Color::Grey;
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
		april::Color color = this->mTextColor;
		unsigned char alpha = this->mBackgroundColor.a;
		if (!this->isDerivedEnabled())
		{
			this->mTextColor = this->mDisabledTextColor;
		}
		else if (this->mHovered)
		{
			this->mBackgroundColor.a = (unsigned char)(this->mBackgroundColor.a * 0.75f);
			if (this->mPushed)
			{
				this->mTextColor = this->mPushedTextColor;
			}
			else if (aprilui::isHoverEffectEnabled())
			{
				this->mTextColor = this->mHoverTextColor;
			}
		}
		Label::OnDraw();
		this->mBackgroundColor.a = alpha;
		this->mTextColor = color;
	}
	
	hstr TextButton::getProperty(chstr name, bool* property_exists)
	{
		if (name == "hover_text_color")		return this->getHoverTextColor().hex();
		if (name == "pushed_text_color")	return this->getPushedTextColor().hex();
		if (name == "disabled_text_color")	return this->getDisabledTextColor().hex();
		if (name == "hover_color")
		{
			hlog::warn(aprilui::logTag, "'hover_color' is deprecated, use 'hover_text_color' instead!"); // DEPRECATED
			return this->getHoverTextColor().hex();
		}
		if (name == "pushed_color")
		{
			hlog::warn(aprilui::logTag, "'pushed_color' is deprecated, use 'pushed_text_color' instead!"); // DEPRECATED
			return this->getPushedTextColor().hex();
		}
		if (name == "disabled_color")
		{
			hlog::warn(aprilui::logTag, "'disabled_color' is deprecated, use 'disabled_text_color' instead!"); // DEPRECATED
			return this->getDisabledTextColor().hex();
		}
		return Label::getProperty(name, property_exists);
	}
	
	bool TextButton::setProperty(chstr name, chstr value)
	{
		if		(name == "hover_text_color")	this->setHoverTextColor(value);
		else if (name == "pushed_text_color")	this->setPushedTextColor(value);
		else if (name == "disabled_text_color")	this->setDisabledTextColor(value);
		else if (name == "hover_color")
		{
			hlog::warn(aprilui::logTag, "'hover_color=' is deprecated, use 'hover_text_color=' instead!"); // DEPRECATED
			this->setHoverTextColor(value);
		}
		else if (name == "pushed_color")
		{
			hlog::warn(aprilui::logTag, "'pushed_color=' is deprecated, use 'pushed_text_color=' instead!"); // DEPRECATED
			this->setPushedTextColor(value);
		}
		else if (name == "disabled_color")
		{
			hlog::warn(aprilui::logTag, "'disabled_color=' is deprecated, use 'disabled_text_color=' instead!"); // DEPRECATED
			this->setDisabledTextColor(value);
		}
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
			this->triggerEvent("MouseDown", button);
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
			this->triggerEvent("Click", button);
		}
		return result;
	}

	bool TextButton::onMouseMove()
	{
		ButtonBase::onMouseMove();
		return Object::onMouseMove();
	}

	void TextButton::cancelMouseDown()
	{
		ButtonBase::cancelMouseDown();
		Object::cancelMouseDown();
	}

}

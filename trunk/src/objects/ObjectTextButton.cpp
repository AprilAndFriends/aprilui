/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.6
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
		this->mPushedTextColor = april::Color::White * 0.2f;
		this->mHoverTextColor = april::Color::Grey;
		this->mDisabledTextColor = april::Color::Grey;
	}
	
	TextButton::TextButton(chstr name, grect rect, bool warningless_internal_hack) :
		Label(name, rect),
		ButtonBase()
	{
		hlog::warn(aprilui::logTag, "'TextButton' is deprecated, use 'TextImageButton' without images instead!"); // DEPRECATED
		this->mText = "TextButton: " + name;
		this->mBackgroundColor = april::Color::Black;
		this->mPushedTextColor = april::Color::White * 0.2f;
		this->mHoverTextColor = april::Color::Grey;
		this->mDisabledTextColor = april::Color::Grey;
	}

	TextButton::~TextButton()
	{
		
	}

	Object* TextButton::createInstance(chstr name, grect rect)
	{
		return new TextButton(name, rect, true);
	}

	hstr TextButton::getName()
	{
		return Label::getName();
	}

	int TextButton::getFocusIndex()
	{
		return Label::getFocusIndex();
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
		Label::update(k);
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
	
	hstr TextButton::getProperty(chstr name, bool* propertyExists)
	{
		if (propertyExists != NULL)
		{
			*propertyExists = true;
		}
		if (name == "hover_text_color")		return this->getHoverTextColor().hex();
		if (name == "pushed_text_color")	return this->getPushedTextColor().hex();
		if (name == "disabled_text_color")	return this->getDisabledTextColor().hex();
		bool exists = false;
		hstr result = ButtonBase::getProperty(name, &exists);
		if (!exists)
		{
			result = Label::getProperty(name, &exists);
		}
		if (propertyExists != NULL)
		{
			*propertyExists = exists;
		}
		return result;
	}
	
	bool TextButton::setProperty(chstr name, chstr value)
	{
		if		(name == "hover_text_color")	this->setHoverTextColor(value);
		else if (name == "pushed_text_color")	this->setPushedTextColor(value);
		else if (name == "disabled_text_color")	this->setDisabledTextColor(value);
		else if (ButtonBase::setProperty(name, value)) { }
		else return Label::setProperty(name, value);
		return true;
	}
	
	bool TextButton::isCursorInside()
	{
		return Label::isCursorInside();
	}

	bool TextButton::onMouseDown(april::Key keyCode)
	{
		if (Label::onMouseDown(keyCode))
		{
			return true;
		}
		bool result = ButtonBase::onMouseDown(keyCode);
		if (result)
		{
			this->triggerEvent("MouseDown", keyCode);
		}
		return result;
	}

	bool TextButton::onMouseUp(april::Key keyCode)
	{
		if (Label::onMouseUp(keyCode))
		{
			return true;
		}
		bool result = ButtonBase::onMouseUp(keyCode);
		if (result)
		{
			this->triggerEvent("Click", keyCode);
		}
		return result;
	}

	bool TextButton::onMouseMove()
	{
		return (Label::onMouseMove() || ButtonBase::onMouseMove());
	}

	bool TextButton::onButtonDown(april::Button buttonCode)
	{
		if (Label::onButtonDown(buttonCode))
		{
			return true;
		}
		bool result = ButtonBase::onButtonDown(buttonCode);
		if (result)
		{
			this->triggerEvent("ButtonDown", buttonCode);
		}
		return result;
	}

	bool TextButton::onButtonUp(april::Button buttonCode)
	{
		if (Label::onButtonUp(buttonCode))
		{
			return true;
		}
		bool click = ButtonBase::onButtonUp(buttonCode);
		bool up = false;
		if (this->mHovered)
		{
			up = this->triggerEvent("ButtonUp", buttonCode);
		}
		if (click)
		{
			this->triggerEvent("ButtonClick", buttonCode);
		}
		return (click || up);
	}

	void TextButton::mouseCancel()
	{
		ButtonBase::mouseCancel();
		Label::mouseCancel();
	}

}

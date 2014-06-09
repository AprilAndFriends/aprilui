/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

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
	harray<PropertyDescription> TextImageButton::_propertyDescriptions;

	TextImageButton::TextImageButton(chstr name, grect rect) :
		LabelBase(),
		ImageButton(name, rect)
	{
		this->text = "TextImageButton: " + name;
		this->pushedTextColor = april::Color::White * 0.2f;
		this->hoverTextColor = april::Color::Grey;
		this->disabledTextColor = april::Color::Grey;
		this->_useHoverTextColor = false;
		this->_usePushedTextColor = false;
		this->_useDisabledTextColor = false;
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

	void TextImageButton::setHoverTextColor(april::Color value)
	{
		this->hoverTextColor = value;
		this->_useHoverTextColor = true;
	}

	void TextImageButton::setPushedTextColor(april::Color value)
	{
		this->pushedTextColor = value;
		this->_usePushedTextColor = true;
	}

	void TextImageButton::setDisabledTextColor(april::Color value)
	{
		this->disabledTextColor = value;
		this->_useDisabledTextColor = true;
	}

	harray<PropertyDescription> TextImageButton::getPropertyDescriptions()
	{
		if (TextImageButton::_propertyDescriptions.size() == 0)
		{
			TextImageButton::_propertyDescriptions += PropertyDescription("hover_text_color", PropertyDescription::TYPE_HEXCOLOR);
			TextImageButton::_propertyDescriptions += PropertyDescription("pushed_text_color", PropertyDescription::TYPE_HEXCOLOR);
			TextImageButton::_propertyDescriptions += PropertyDescription("disabled_text_color", PropertyDescription::TYPE_HEXCOLOR);
		}
		return (LabelBase::getPropertyDescriptions() + ImageButton::getPropertyDescriptions() + TextImageButton::_propertyDescriptions);
	}

	void TextImageButton::OnDraw()
	{
		ImageButton::OnDraw();
		april::Color color = this->textColor;
		april::Color drawColor = this->_getDrawColor();
		unsigned char alpha = this->backgroundColor.a;
		if (!this->isDerivedEnabled())
		{
			if (this->_useDisabledTextColor)
			{
				this->textColor = this->disabledTextColor;
			}
			else
			{
				drawColor.a = (unsigned char)(drawColor.a * this->_getDisabledAlphaFactor());
			}
		}
		else if (this->hovered)
		{
			this->backgroundColor.a = (unsigned char)(this->backgroundColor.a * 0.75f);
			if (this->pushed)
			{
				if (this->_usePushedTextColor)
				{
					this->textColor = this->pushedTextColor;
				}
			}
			else if (aprilui::isHoverEffectEnabled() && this->_useHoverTextColor)
			{
				this->textColor = this->hoverTextColor;
			}
		}
		grect rect = this->_getDrawRect();
		this->backgroundColor.a = (unsigned char)(this->backgroundColor.a * this->_getDisabledAlphaFactor());
		LabelBase::_drawLabel(rect, drawColor, this->backgroundColor);
		this->textColor = color;
		this->backgroundColor.a = alpha;
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

	bool TextImageButton::triggerEvent(chstr name, april::Button buttonCode, chstr extra)
	{
		return ImageButton::triggerEvent(name, buttonCode, extra);
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
		if		(name == "hover_text_color")	this->setHoverTextColor(value);
		else if (name == "pushed_text_color")	this->setPushedTextColor(value);
		else if (name == "disabled_text_color")	this->setDisabledTextColor(value);
		else if (LabelBase::setProperty(name, value)) { }
		else return ImageButton::setProperty(name, value);
		return true;
	}
	
}

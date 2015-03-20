/// @file
/// @version 4.0
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

	TextImageButton::TextImageButton(chstr name) : ImageButton(name), LabelBase()
	{
		this->text = "TextImageButton: " + name;
		this->pushedTextColor = april::Color::White * 0.2f;
		this->hoverTextColor = april::Color::Grey;
		this->disabledTextColor = april::Color::Grey;
		this->_useHoverTextColor = false;
		this->_usePushedTextColor = false;
		this->_useDisabledTextColor = false;
	}

	TextImageButton::TextImageButton(const TextImageButton& other) : ImageButton(other), LabelBase(other)
	{
		this->pushedTextColor = other.pushedTextColor;
		this->hoverTextColor = other.hoverTextColor;
		this->disabledTextColor = other.disabledTextColor;
		this->_useHoverTextColor = other._useHoverTextColor;
		this->_usePushedTextColor = other._usePushedTextColor;
		this->_useDisabledTextColor = other._useDisabledTextColor;
	}

	TextImageButton::~TextImageButton()
	{
	}

	Object* TextImageButton::createInstance(chstr name)
	{
		return new TextImageButton(name);
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
			TextImageButton::_propertyDescriptions += PropertyDescription("hover_text_color", PropertyDescription::HEXCOLOR);
			TextImageButton::_propertyDescriptions += PropertyDescription("pushed_text_color", PropertyDescription::HEXCOLOR);
			TextImageButton::_propertyDescriptions += PropertyDescription("disabled_text_color", PropertyDescription::HEXCOLOR);
		}
		return (LabelBase::getPropertyDescriptions() + ImageButton::getPropertyDescriptions() + TextImageButton::_propertyDescriptions);
	}

	void TextImageButton::_draw()
	{
		ImageButton::_draw();
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
		LabelBase::_drawLabelBackground(rect, drawColor, this->backgroundColor);
		LabelBase::_drawLabel(rect, drawColor);
		this->textColor = color;
		this->backgroundColor.a = alpha;
	}
	
	void TextImageButton::notifyEvent(chstr type, EventArgs* args)
	{
		LabelBase::notifyEvent(type, args);
		ImageButton::notifyEvent(type, args);
	}

	bool TextImageButton::triggerEvent(chstr type, april::Key keyCode)
	{
		return ImageButton::triggerEvent(type, keyCode);
	}

	bool TextImageButton::triggerEvent(chstr type, april::Key keyCode, chstr string)
	{
		return Object::triggerEvent(type, keyCode, string);
	}

	bool TextImageButton::triggerEvent(chstr type, april::Key keyCode, gvec2 position, chstr string, void* userData)
	{
		return ImageButton::triggerEvent(type, keyCode, position, string, userData);
	}

	bool TextImageButton::triggerEvent(chstr type, april::Button buttonCode, chstr string, void* userData)
	{
		return ImageButton::triggerEvent(type, buttonCode, string, userData);
	}

	bool TextImageButton::triggerEvent(chstr type, chstr string, void* userData)
	{
		return ImageButton::triggerEvent(type, string, userData);
	}

	bool TextImageButton::triggerEvent(chstr type, void* userData)
	{
		return ImageButton::triggerEvent(type, userData);
	}

	hstr TextImageButton::getProperty(chstr name)
	{
		if (name == "hover_text_color")		return this->getHoverTextColor().hex();
		if (name == "pushed_text_color")	return this->getPushedTextColor().hex();
		if (name == "disabled_text_color")	return this->getDisabledTextColor().hex();
		hstr result = LabelBase::getProperty(name);
		if (result == "")
		{
			result = ImageButton::getProperty(name);
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

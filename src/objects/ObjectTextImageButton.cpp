/// @file
/// @version 5.0
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
#include "apriluiUtil.h"
#include "Dataset.h"
#include "ObjectLabelBase.h"
#include "ObjectTextImageButton.h"

namespace aprilui
{
	harray<PropertyDescription> TextImageButton::_propertyDescriptions;
	hmap<hstr, PropertyDescription::Accessor*> TextImageButton::_getters;
	hmap<hstr, PropertyDescription::Accessor*> TextImageButton::_setters;

	TextImageButton::TextImageButton(chstr name) : ImageButton(name), LabelBase()
	{
		this->text = "TextImageButton: " + name;
		this->hoverTextColor = april::Color::Grey;
		this->pushedTextColor = april::Color::White * 0.2f;
		this->disabledTextColor = april::Color::Grey;
		this->_usePushedTextColor = false;
		this->_useHoverTextColor = false;
		this->_useDisabledTextColor = false;
	}

	TextImageButton::TextImageButton(const TextImageButton& other) : ImageButton(other), LabelBase(other)
	{
		this->pushedTextColor = other.pushedTextColor;
		this->hoverTextColor = other.hoverTextColor;
		this->disabledTextColor = other.disabledTextColor;
		this->_usePushedTextColor = other._usePushedTextColor;
		this->_useHoverTextColor = other._useHoverTextColor;
		this->_useDisabledTextColor = other._useDisabledTextColor;
	}

	TextImageButton::~TextImageButton()
	{
	}

	Object* TextImageButton::createInstance(chstr name)
	{
		return new TextImageButton(name);
	}

	harray<PropertyDescription> TextImageButton::getPropertyDescriptions() const
	{
		if (TextImageButton::_propertyDescriptions.size() == 0)
		{
			TextImageButton::_propertyDescriptions = ImageButton::getPropertyDescriptions() + LabelBase::getPropertyDescriptions();
			TextImageButton::_propertyDescriptions += PropertyDescription("pushed_text_color", PropertyDescription::Type::Color);
			TextImageButton::_propertyDescriptions += PropertyDescription("hover_text_color", PropertyDescription::Type::Color);
			TextImageButton::_propertyDescriptions += PropertyDescription("disabled_text_color", PropertyDescription::Type::Color);
		}
		return TextImageButton::_propertyDescriptions;
	}

	hmap<hstr, PropertyDescription::Accessor*>& TextImageButton::_getGetters() const
	{
		if (TextImageButton::_getters.size() == 0)
		{
			TextImageButton::_getters = ImageButton::_getGetters() + LabelBase::_generateGetters<TextImageButton>();
			TextImageButton::_getters["pushed_text_color"] = new PropertyDescription::GetColor<TextImageButton>(&TextImageButton::getPushedTextColor);
			TextImageButton::_getters["hover_text_color"] = new PropertyDescription::GetColor<TextImageButton>(&TextImageButton::getHoverTextColor);
			TextImageButton::_getters["disabled_text_color"] = new PropertyDescription::GetColor<TextImageButton>(&TextImageButton::getDisabledTextColor);
		}
		return TextImageButton::_getters;
	}

	hmap<hstr, PropertyDescription::Accessor*>& TextImageButton::_getSetters() const
	{
		if (TextImageButton::_setters.size() == 0)
		{
			TextImageButton::_setters = ImageButton::_getSetters() + LabelBase::_generateSetters<TextImageButton>();
			TextImageButton::_setters["pushed_text_color"] = new PropertyDescription::Set<TextImageButton, hstr>(&TextImageButton::setPushedTextSymbolicColor);
			TextImageButton::_setters["hover_text_color"] = new PropertyDescription::Set<TextImageButton, hstr>(&TextImageButton::setHoverTextSymbolicColor);
			TextImageButton::_setters["disabled_text_color"] = new PropertyDescription::Set<TextImageButton, hstr>(&TextImageButton::setDisabledTextSymbolicColor);
		}
		return TextImageButton::_setters;
	}

	Dataset* TextImageButton::getDataset() const
	{
		return ImageButton::getDataset();
	}

	hstr TextImageButton::getAutoScaledFont()
	{
		this->_calcAutoScaledFont(this->_makeDrawRect());
		return this->autoScaledFont;
	}

	void TextImageButton::setHoverTextColor(const april::Color& value)
	{
		this->hoverTextColor = value;
		this->_useHoverTextColor = true;
	}

	void TextImageButton::setHoverTextSymbolicColor(chstr value)
	{
		this->setHoverTextColor(aprilui::_makeColor(value));
	}

	void TextImageButton::setPushedTextColor(const april::Color& value)
	{
		this->pushedTextColor = value;
		this->_usePushedTextColor = true;
	}

	void TextImageButton::setPushedTextSymbolicColor(chstr value)
	{
		this->setPushedTextColor(aprilui::_makeColor(value));
	}

	void TextImageButton::setDisabledTextColor(const april::Color& value)
	{
		this->disabledTextColor = value;
		this->_useDisabledTextColor = true;
	}

	void TextImageButton::setDisabledTextSymbolicColor(chstr value)
	{
		this->setDisabledTextColor(aprilui::_makeColor(value));
	}

	void TextImageButton::_draw()
	{
		ImageButton::_draw();
		bool useDisabledAlpha = this->useDisabledAlpha;
		this->useDisabledAlpha = !this->_useDisabledTextColor;
		april::Color backgroundColor = this->backgroundColor;
		april::Color textColor = this->textColor;
		april::Color drawLabelColor = this->_makeDrawColor();
		this->useDisabledAlpha = useDisabledAlpha;
		bool applyHoverAlpha = false;
		if (!this->isDerivedEnabled())
		{
			if (this->_useDisabledTextColor)
			{
				this->textColor = this->disabledTextColor;
			}
			if (this->_useDisabledColor)
			{
				this->backgroundColor = this->disabledColor;
			}
		}
		else if (this->hovered)
		{
			applyHoverAlpha = true;
			if (this->pushed)
			{
				if (this->_usePushedTextColor)
				{
					this->textColor = this->pushedTextColor;
				}
				if (this->_usePushedColor)
				{
					this->backgroundColor = this->pushedColor;
					applyHoverAlpha = false;
				}
			}
			else if (aprilui::isHoverEffectEnabled())
			{
				if (this->_useHoverTextColor)
				{
					this->textColor = this->hoverTextColor;
				}
				if (this->_useHoverColor)
				{
					this->backgroundColor = this->hoverColor;
					applyHoverAlpha = false;
				}
			}
		}
		april::Color backgroundDrawColor = this->_makeBackgroundDrawColor(this->_makeDrawColor());
		if (applyHoverAlpha)
		{
			backgroundDrawColor.a = (unsigned char)(backgroundDrawColor.a * 0.75f);
		}
		grect drawRect = this->_makeDrawRect();
		LabelBase::_drawLabelBackground(drawRect, drawLabelColor, backgroundDrawColor);
		LabelBase::_drawLabel(drawRect, drawLabelColor);
		this->backgroundColor = backgroundColor;
		this->textColor = textColor;
	}
	
	hstr TextImageButton::getProperty(chstr name)
	{
		hstr result = LabelBase::getProperty(name); // check side-class first
		if (result == "")
		{
			result = ImageButton::getProperty(name);
		}
		return result;
	}

	bool TextImageButton::setProperty(chstr name, chstr value)
	{
		if (LabelBase::setProperty(name, value)) // check side-class first
		{
			return true;
		}
		return ImageButton::setProperty(name, value);
	}

	void TextImageButton::notifyEvent(chstr type, EventArgs* args)
	{
		ImageButton::notifyEvent(type, args);
		LabelBase::notifyEvent(type, args);
	}

	bool TextImageButton::triggerEvent(chstr type, april::Key keyCode)
	{
		return ImageButton::triggerEvent(type, keyCode);
	}

	bool TextImageButton::triggerEvent(chstr type, april::Key keyCode, chstr string)
	{
		return ImageButton::triggerEvent(type, keyCode, string);
	}

	bool TextImageButton::triggerEvent(chstr type, april::Key keyCode, cgvec2 position, chstr string, void* userData)
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

}

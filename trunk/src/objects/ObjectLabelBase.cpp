/// @file
/// @version 3.6
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/Color.h>
#include <april/RenderSystem.h>
#include <atres/atres.h>
#include <gtypes/Rectangle.h>
#include <hltypes/exception.h>
#include <hltypes/harray.h>
#include <hltypes/hlog.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "apriluiUtil.h"
#include "Dataset.h"
#include "Exception.h"
#include "ObjectLabelBase.h"

namespace aprilui
{
	harray<PropertyDescription> LabelBase::_propertyDescriptions;

	LabelBase::LabelBase()
	{
		this->text = "";
		this->textKey = "";
		this->font = "";
		this->textFormatting = true;
		this->textColor = april::Color::White;
		this->textOffset.set(0.0f, 0.0f);
		this->horzFormatting = atres::CENTER_WRAPPED;
		this->vertFormatting = atres::CENTER;
		this->fontEffect = atres::NONE;
		this->useFontEffectColor = false;
		this->fontEffectColor = april::Color::Black;
		this->backgroundColor = april::Color::Clear;
		this->backgroundBorder = true;
	}

	LabelBase::~LabelBase()
	{
	}

	harray<PropertyDescription> LabelBase::getPropertyDescriptions()
	{
		if (LabelBase::_propertyDescriptions.size() == 0)
		{
			LabelBase::_propertyDescriptions += PropertyDescription("font", PropertyDescription::STRING);
			LabelBase::_propertyDescriptions += PropertyDescription("text", PropertyDescription::STRING);
			LabelBase::_propertyDescriptions += PropertyDescription("text_key", PropertyDescription::STRING);
			LabelBase::_propertyDescriptions += PropertyDescription("horz_formatting", PropertyDescription::ENUM);
			LabelBase::_propertyDescriptions += PropertyDescription("vert_formatting", PropertyDescription::ENUM);
			LabelBase::_propertyDescriptions += PropertyDescription("text_color", PropertyDescription::HEXCOLOR);
			LabelBase::_propertyDescriptions += PropertyDescription("effect", PropertyDescription::ENUM);
			LabelBase::_propertyDescriptions += PropertyDescription("text_offset", PropertyDescription::GVEC2);
			LabelBase::_propertyDescriptions += PropertyDescription("text_offset_x", PropertyDescription::FLOAT);
			LabelBase::_propertyDescriptions += PropertyDescription("text_offset_y", PropertyDescription::FLOAT);
			LabelBase::_propertyDescriptions += PropertyDescription("background_color", PropertyDescription::HEXCOLOR);
			LabelBase::_propertyDescriptions += PropertyDescription("background_border", PropertyDescription::BOOL);
		}
		return LabelBase::_propertyDescriptions;
	}

	void LabelBase::_drawLabelBackground(grect rect, april::Color color, april::Color backgroundColor)
	{
		if (backgroundColor.a > 0)
		{
			april::rendersys->drawFilledRect(rect, backgroundColor);
			if (this->backgroundBorder)
			{
				april::rendersys->drawRect(rect, april::Color(color, backgroundColor.a));
			}
		}
	}

	void LabelBase::_drawLabel(grect rect, april::Color color)
	{
		if (this->text.size() == 0)
		{
			return;
		}
		color *= this->textColor;
		hstr text = this->text;
		if (!this->textFormatting)
		{
			text = "[-]" + text;
		}
		hstr colorCode = "";
		if (this->useFontEffectColor)
		{
			colorCode += ":" + this->fontEffectColor.hex();
		}
		switch (this->fontEffect)
		{
		case atres::BORDER:
			text = "[b" + colorCode + "]" + text;
			break;
		case atres::SHADOW:
			text = "[s" + colorCode + "]" + text;
			break;
		default:
			break;
		}
		atres::renderer->drawText(this->font, rect, text, this->horzFormatting, this->vertFormatting, color, -this->textOffset);
	}

	hstr LabelBase::getProperty(chstr name)
	{
		if (name == "font")					return this->getFont();
		if (name == "text")					return this->getText();
		if (name == "text_key")				return this->getTextKey();
		if (name == "horz_formatting")
		{
			if (this->horzFormatting == atres::LEFT)			return "left";
			if (this->horzFormatting == atres::RIGHT)			return "right";
			if (this->horzFormatting == atres::CENTER)			return "center";
			if (this->horzFormatting == atres::LEFT_WRAPPED)	return "left_wrapped";
			if (this->horzFormatting == atres::RIGHT_WRAPPED)	return "right_wrapped";
			if (this->horzFormatting == atres::CENTER_WRAPPED)	return "center_wrapped";
			if (this->horzFormatting == atres::JUSTIFIED)		return "justified";
		}
		if (name == "vert_formatting")
		{
			if (this->vertFormatting == atres::TOP)		return "top";
			if (this->vertFormatting == atres::CENTER)	return "center";
			if (this->vertFormatting == atres::BOTTOM)	return "bottom";
		}
		if (name == "text_color")			return this->getTextColor().hex();
		if (name == "effect")
		{
			hstr effect = "";
			if (this->fontEffect == atres::SHADOW)	effect = "shadow";
			if (this->fontEffect == atres::BORDER)	effect = "border";
			if (this->fontEffect == atres::NONE)	effect = "none";
			if (this->useFontEffectColor)
			{
				effect += ":" + this->fontEffectColor.hex();
			}
			return effect;
		}
		if (name == "text_offset")			return april::gvec2ToHstr(this->getTextOffset());
		if (name == "text_offset_x")		return this->getTextOffsetX();
		if (name == "text_offset_y")		return this->getTextOffsetY();
		if (name == "background_color")		return this->getBackgroundColor().hex();
		if (name == "background_border")	return this->isBackgroundBorder();
		return "";
	}
	
	bool LabelBase::setProperty(chstr name, chstr value)
	{
		if (name == "font")						this->setFont(value);
		else if (name == "text_key")			this->setTextKey(value);
		else if (name == "text")				this->setText(value);
		else if (name == "horz_formatting")
		{
			if (value == "left")				this->setHorzFormatting(atres::LEFT);
			else if (value == "right")			this->setHorzFormatting(atres::RIGHT);
			else if (value == "center")			this->setHorzFormatting(atres::CENTER);
			else if (value == "left_wrapped")	this->setHorzFormatting(atres::LEFT_WRAPPED);
			else if (value == "right_wrapped")	this->setHorzFormatting(atres::RIGHT_WRAPPED);
			else if (value == "center_wrapped")	this->setHorzFormatting(atres::CENTER_WRAPPED);
			else if (value == "justified")		this->setHorzFormatting(atres::JUSTIFIED);
			else
			{
				hlog::warn(aprilui::logTag, "'horz_formatting=' does not support value '" + value + "'.");
				return false;
			}
		}
		else if (name == "vert_formatting")
		{
			if (value == "top")			this->setVertFormatting(atres::TOP);
			else if (value == "center")	this->setVertFormatting(atres::CENTER);
			else if (value == "bottom")	this->setVertFormatting(atres::BOTTOM);
			else
			{
				hlog::warn(aprilui::logTag, "'vert_formatting=' does not support value '" + value + "'.");
				return false;
			}
		}
		else if (name == "text_color")			this->setTextColor(value);
		else if (name == "effect")
		{
			this->setFontEffect(atres::NONE);
			this->setUseFontEffectColor(false);
			harray<hstr> values = value.split(":", -1, true);
			if (values.size() > 0)
			{
				if (values[0] == "none")		this->setFontEffect(atres::NONE);
				else if (values[0] == "shadow")	this->setFontEffect(atres::SHADOW);
				else if (values[0] == "border")	this->setFontEffect(atres::BORDER);
				else
				{
					hlog::warn(aprilui::logTag, "'effect=' does not support value '" + values[0] + "'.");
					return false;
				}
				if (values.size() > 1)
				{
					if (values[1].is_hex() && (values[1].size() == 6 || values[1].size() == 8))
					{
						this->setUseFontEffectColor(true);
						this->setFontEffectColor(values[1]);
					}
					else
					{
						hlog::warn(aprilui::logTag, "'effect=' is using invalid color modifier '" + values[1] + "'.");
						return false;
					}
				}
			}
		}
		else if (name == "text_offset")			this->setTextOffset(april::hstrToGvec2(value));
		else if (name == "text_offset_x")		this->setTextOffsetX(value);
		else if (name == "text_offset_y")		this->setTextOffsetY(value);
		else if (name == "background_color")	this->setBackgroundColor(value);
		else if (name == "background_border")	this->setBackgroundBorder(value);
		else return false;
		return true;
	}
	
	void LabelBase::notifyEvent(chstr type, EventArgs* args)
	{
		if (type == Event::LocalizationChanged)
		{
			if (this->textKey != "")
			{
				this->setTextKey(this->textKey);
			}
		}
	}
	
	void LabelBase::setText(chstr value)
	{
		bool changed = (this->text != value);
		this->text = value;
		this->textKey = "";
		if (changed)
		{
			this->triggerEvent(Event::TextChanged);
		}
	}
	
	void LabelBase::setTextKey(chstr value)
	{
		bool changed = (this->textKey != value);
		hstr newTextKey = value; // because value is a chstr which could reference this->textKey itself
		this->setText(this->getDataset()->getText(newTextKey));
		this->textKey = newTextKey;
		if (changed)
		{
			this->triggerEvent(Event::TextKeyChanged);
		}
	}
	
	bool LabelBase::trySetTextKey(chstr textKey)
	{
		if (this->textKey != textKey)
		{
			hstr newTextKey = textKey; // because value is a chstr which could reference this->textKey itself
			this->setText(this->getDataset()->getText(newTextKey));
			this->textKey = newTextKey;
			this->triggerEvent(Event::TextKeyChanged);
			return true;
		}
		return false;
	}

}

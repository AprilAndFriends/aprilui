/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.25
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <april/Color.h>
#include <april/RenderSystem.h>
#include <atres/atres.h>
#include <gtypes/Rectangle.h>
#include <hltypes/exception.h>
#include <hltypes/harray.h>
#include <hltypes/hlog.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "Dataset.h"
#include "Exception.h"
#include "ObjectLabelBase.h"

namespace aprilui
{
	LabelBase::LabelBase()
	{
		this->mText = "";
		this->mTextKey = "";
		this->mFontName = "";
		this->mTextFormatting = true;
		this->mTextColor = APRIL_COLOR_WHITE;
		this->mDrawOffset.set(0.0f, 0.0f);
		this->mHorzFormatting = atres::CENTER_WRAPPED;
		this->mVertFormatting = atres::CENTER;
		this->mFontEffect = atres::NONE;
		this->mUseFontEffectColor = false;
		this->mFontEffectColor = APRIL_COLOR_BLACK;
	}

	LabelBase::~LabelBase()
	{
	}

	void LabelBase::_drawLabel(grect rect, april::Color color)
	{
		color *= this->mTextColor;
		if (aprilui::isDebugEnabled())
		{
			unsigned char alpha = color.a / 2;
			april::rendersys->drawFilledRect(rect, april::Color(APRIL_COLOR_BLACK, alpha));
			april::rendersys->drawRect(rect, april::Color(APRIL_COLOR_WHITE, alpha));
		}
		if (this->mText.size() == 0)
		{
			return;
		}
		hstr text = this->mText;
		hstr colorCode = "";
		if (this->mUseFontEffectColor)
		{
			colorCode += ":" + this->mFontEffectColor.hex();
		}
		switch (this->mFontEffect)
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
		if (this->mTextFormatting)
		{
			atres::renderer->drawText(this->mFontName, rect, text, this->mHorzFormatting, this->mVertFormatting, color, -this->mDrawOffset);
		}
		else
		{
			atres::renderer->drawTextUnformatted(this->mFontName, rect, text, this->mHorzFormatting, this->mVertFormatting, color, -this->mDrawOffset);
		}
	}

	hstr LabelBase::getProperty(chstr name, bool* property_exists)
	{
		if (property_exists != NULL)
		{
			*property_exists = true;
		}
		if (name == "font")			return this->getFont();
		if (name == "text")			return this->getText();
		if (name == "text_key")		return this->getTextKey();
		if (name == "horz_formatting")
		{
			if (this->mHorzFormatting == atres::LEFT)			return "left";
			if (this->mHorzFormatting == atres::RIGHT)			return "right";
			if (this->mHorzFormatting == atres::CENTER)			return "center";
			if (this->mHorzFormatting == atres::LEFT_WRAPPED)	return "left_wrapped";
			if (this->mHorzFormatting == atres::RIGHT_WRAPPED)	return "right_wrapped";
			if (this->mHorzFormatting == atres::CENTER_WRAPPED)	return "center_wrapped";
			if (this->mHorzFormatting == atres::JUSTIFIED)		return "justified";
		}
		if (name == "vert_formatting")
		{
			if (this->mVertFormatting == atres::TOP)	return "top";
			if (this->mVertFormatting == atres::CENTER)	return "center";
			if (this->mVertFormatting == atres::BOTTOM)	return "bottom";
		}
		if (name == "text_color")	return this->getTextColor().hex();
		if (name == "effect")
		{
			hstr effect = "";
			if (this->mFontEffect == atres::SHADOW)	effect = "shadow";
			if (this->mFontEffect == atres::BORDER)	effect = "border";
			if (this->mFontEffect == atres::NONE)	effect = "none";
			if (this->mUseFontEffectColor)
			{
				effect += ":" + this->mFontEffectColor.hex();
			}
			return effect;
		}
		if (name == "offset_x")		return this->mDrawOffset.x;
		if (name == "offset_y")		return this->mDrawOffset.y;
		if (property_exists != NULL)
		{
			*property_exists = false;
		}
		return "";
	}
	
	bool LabelBase::setProperty(chstr name, chstr value)
	{
		if (name == "font")				this->setFont(value);
		else if (name == "text_key")	this->setTextKey(value);
		else if (name == "textkey")
		{
			hlog::warn(aprilui::logTag, "'textkey=' is deprecated. Use 'text_key=' instead."); // DEPRECATED
			this->setTextKey(value);
		}
		else if (name == "text")		this->setText(value);
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
		else if (name == "text_color")	this->setTextColor(value);
		else if (name == "color")
		{
			throw hl_exception("LabelBase instance using 'color=' which is conflicted with TextImageButton's color and cannot be used! Maybe you meant 'text_color='?");
		}
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
		else if (name == "offset_x")	this->mDrawOffset.x = (float)value;
		else if (name == "offset_y")	this->mDrawOffset.y = (float)value;
		else return false;
		return true;
	}
	
	void LabelBase::notifyEvent(chstr name, void* params)
	{
		if (name == "onLocalizationChanged")
		{
			if (this->mTextKey != "")
			{
				this->setTextKey(this->mTextKey);
			}
		}
	}
	
	void LabelBase::setText(chstr value)
	{
		this->mText = value;
		this->mTextKey = "";
	}

	void LabelBase::setTextKey(chstr value)
	{
		hstr textKey = value; // because value is a chstr which could reference mTextKey
		this->setText(this->getDataset()->getText(textKey));
		this->mTextKey = textKey;
	}

}

/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.91
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
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "Dataset.h"
#include "Exception.h"
#include "ObjectLabelBase.h"

namespace aprilui
{
	LabelBase::LabelBase()
	{
		this->mHorzFormatting = atres::CENTER_WRAPPED;
		this->mVertFormatting = atres::CENTER;
		this->mFontEffect = atres::NONE;
		this->mTextFormatting = true;
		this->mText = "";
		this->mTextKey = "";
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
		switch (this->mFontEffect)
		{
		case atres::BORDER:
			text = "[b]" + text;
			break;
		case atres::SHADOW:
			text = "[s]" + text;
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
			if (this->mVertFormatting == atres::TOP)		return "top";
			if (this->mVertFormatting == atres::CENTER)	return "center";
			if (this->mVertFormatting == atres::BOTTOM)	return "bottom";
		}
		if (name == "text_color")	return this->getTextColor().hex();
		if (name == "effect")
		{
			if (this->mFontEffect == atres::SHADOW)	return "shadow";
			if (this->mFontEffect == atres::BORDER)	return "border";
			if (this->mFontEffect == atres::NONE)	return "none";
		}
		if (name == "offset_x")		return this->mDrawOffset.x;
		if (name == "offset_y")		return this->mDrawOffset.y;
		if (property_exists != NULL)
		{
			*property_exists = false;
		}
		return "";
	}
	
	bool LabelBase::setProperty(chstr name,chstr value)
	{
		if (name == "font")				this->setFont(value);
		else if (name == "text_key")	this->setTextKey(value);
		else if (name == "textkey")
		{
			aprilui::log("WARNING: 'textkey=' is deprecated. Use 'text_key=' instead."); // DEPRECATED
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
		}
		else if (name == "vert_formatting")
		{
			if (value == "top")			this->setVertFormatting(atres::TOP);
			else if (value == "center")	this->setVertFormatting(atres::CENTER);
			else if (value == "bottom")	this->setVertFormatting(atres::BOTTOM);
		}
		else if (name == "text_color")	this->setTextColor(value);
		else if (name == "color")
		{
			throw hl_exception("LabelBase instance using 'color=' which is conflicted with TextImageButton's color and cannot be used! Maybe you meant 'text_color='?");
		}
		else if (name == "effect")
		{
			if (value == "none")		this->setFontEffect(atres::NONE);
			else if (value == "shadow")	this->setFontEffect(atres::SHADOW);
			else if (value == "border")	this->setFontEffect(atres::BORDER);
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

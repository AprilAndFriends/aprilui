/// @file
/// @version 4.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/Color.h>
#include <april/RenderSystem.h>
#include <atres/atres.h>
#include <atres/Font.h>
#include <gtypes/Rectangle.h>
#include <hltypes/harray.h>
#include <hltypes/hexception.h>
#include <hltypes/hlog.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "apriluiUtil.h"
#include "Dataset.h"
#include "Exception.h"
#include "ObjectLabelBase.h"

#define MAX_AUTO_SCALE_STEPS 5
#define SAFE_AUTO_SCALE_FACTOR 1.01f // floating point errors can cause problems so the auto-scale required area is slightly enlarged
#define SAFE_AUTO_SCALE_CHECK_VALUE 1.2f // suddenly the calculated height could be much higher

namespace aprilui
{
	float LabelBase::defaultMinAutoScale = 1.0f;
	harray<PropertyDescription> LabelBase::_propertyDescriptions;

	LabelBase::LabelBase()
	{
		this->text = "";
		this->textKey = "";
		this->font = "";
		this->textFormatting = true;
		this->textColor = april::Color::White;
		this->textOffset.set(0.0f, 0.0f);
		this->minAutoScale = LabelBase::defaultMinAutoScale;
		this->horzFormatting = atres::Horizontal::CenterWrapped;
		this->vertFormatting = atres::Vertical::Center;
		this->effect = atres::TextEffect::None;
		this->useEffectColor = false;
		this->useEffectParameter = false;
		this->effectColor = april::Color::Black;
		this->strikeThrough = false;
		this->useStrikeThroughColor = false;
		this->useStrikeThroughParameter = false;
		this->strikeThroughColor = april::Color::White;
		this->underline = false;
		this->useUnderlineColor = false;
		this->useUnderlineParameter = false;
		this->underlineColor = april::Color::White;
		this->backgroundColor = april::Color::Clear;
		this->backgroundBorder = true;
		this->_autoScaleDirty = true;
	}

	LabelBase::LabelBase(const LabelBase& other)
	{
		this->text = other.text;
		this->textKey = other.textKey;
		this->font = other.font;
		this->textFormatting = other.textFormatting;
		this->textColor = other.textColor;
		this->textOffset = other.textOffset;
		this->minAutoScale = other.minAutoScale;
		this->horzFormatting = other.horzFormatting;
		this->vertFormatting = other.vertFormatting;
		this->effect = other.effect;
		this->useEffectColor = other.useEffectColor;
		this->useEffectParameter = other.useEffectParameter;
		this->effectColor = other.effectColor;
		this->effectParameter = other.effectParameter;
		this->useStrikeThroughColor = other.useStrikeThroughColor;
		this->useStrikeThroughParameter = other.useStrikeThroughParameter;
		this->strikeThroughColor = other.strikeThroughColor;
		this->strikeThroughParameter = other.strikeThroughParameter;
		this->underline = other.underline;
		this->useUnderlineColor = other.useUnderlineColor;
		this->useUnderlineParameter = other.useUnderlineParameter;
		this->underlineColor = other.underlineColor;
		this->underlineParameter = other.underlineParameter;
		this->backgroundColor = other.backgroundColor;
		this->backgroundBorder = other.backgroundBorder;
		this->_autoScaleDirty = true;
	}

	LabelBase::~LabelBase()
	{
	}

	harray<PropertyDescription> LabelBase::getPropertyDescriptions() const
	{
		if (LabelBase::_propertyDescriptions.size() == 0)
		{
			LabelBase::_propertyDescriptions += PropertyDescription("font", PropertyDescription::STRING);
			LabelBase::_propertyDescriptions += PropertyDescription("text", PropertyDescription::STRING);
			LabelBase::_propertyDescriptions += PropertyDescription("text_key", PropertyDescription::STRING);
			LabelBase::_propertyDescriptions += PropertyDescription("text_color", PropertyDescription::HEXCOLOR);
			LabelBase::_propertyDescriptions += PropertyDescription("text_offset", PropertyDescription::GVEC2);
			LabelBase::_propertyDescriptions += PropertyDescription("text_offset_x", PropertyDescription::FLOAT);
			LabelBase::_propertyDescriptions += PropertyDescription("text_offset_y", PropertyDescription::FLOAT);
			LabelBase::_propertyDescriptions += PropertyDescription("min_auto_scale", PropertyDescription::FLOAT);
			LabelBase::_propertyDescriptions += PropertyDescription("auto_scaled_font", PropertyDescription::STRING);
			LabelBase::_propertyDescriptions += PropertyDescription("horz_formatting", PropertyDescription::ENUM);
			LabelBase::_propertyDescriptions += PropertyDescription("vert_formatting", PropertyDescription::ENUM);
			LabelBase::_propertyDescriptions += PropertyDescription("effect", PropertyDescription::ENUM);
			LabelBase::_propertyDescriptions += PropertyDescription("strike_through", PropertyDescription::BOOL);
			LabelBase::_propertyDescriptions += PropertyDescription("underline", PropertyDescription::BOOL);
			LabelBase::_propertyDescriptions += PropertyDescription("background_color", PropertyDescription::HEXCOLOR);
			LabelBase::_propertyDescriptions += PropertyDescription("background_border", PropertyDescription::BOOL);
		}
		return LabelBase::_propertyDescriptions;
	}

	void LabelBase::setText(chstr value)
	{
		bool changed = (this->text != value);
		this->text = value;
		this->textKey = "";
		if (changed)
		{
			this->triggerEvent(Event::TextChanged);
			this->_autoScaleDirty = true;
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
			this->_autoScaleDirty = true;
		}
	}

	void LabelBase::setFont(chstr value)
	{
		if (this->font != value)
		{
			this->font = value;
			this->_autoScaleDirty = true;
		}
	}

	void LabelBase::setTextOffset(gvec2 value)
	{
		if (this->textOffset != value)
		{
			this->textOffset = value;
			this->_autoScaleDirty = true;
		}
	}

	void LabelBase::setTextOffsetX(float value)
	{
		if (this->textOffset.x != value)
		{
			this->textOffset.x = value;
			this->_autoScaleDirty = true;
		}
	}

	void LabelBase::setTextOffsetY(float value)
	{
		if (this->textOffset.y != value)
		{
			this->textOffset.y = value;
			this->_autoScaleDirty = true;
		}
	}

	void LabelBase::setMinAutoScale(float value)
	{
		value = hclamp(value, 0.1f, 1.0f);
		if (this->minAutoScale != value)
		{
			this->minAutoScale = value;
			this->_autoScaleDirty = true;
		}
	}

	void LabelBase::_calcAutoScaledFont(grect rect)
	{
		if (this->_autoScaleDirty)
		{
			hstr text = this->text;
			if (!this->textFormatting)
			{
				text = "[-]" + text;
			}
			this->_calcAutoScaleFont(this->font, rect, text, this->horzFormatting, this->vertFormatting);
			this->_autoScaleDirty = false;
		}
	}

	void LabelBase::setHorzFormatting(atres::Horizontal value)
	{
		if (this->horzFormatting != value)
		{
			this->horzFormatting = value;
			this->_autoScaleDirty = true;
		}
	}

	void LabelBase::setVertFormatting(atres::Vertical value)
	{
		if (this->vertFormatting != value)
		{
			this->vertFormatting = value;
			this->_autoScaleDirty = true;
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
			this->_autoScaleDirty = true;
			return true;
		}
		return false;
	}

	april::Color LabelBase::_makeBackgroundDrawColor(april::Color drawColor) const
	{
		return aprilui::makeModifiedDrawColor(this->backgroundColor, drawColor);
	}

	void LabelBase::_drawLabelBackground(grect rect, april::Color color, april::Color backgroundColor)
	{
		if (backgroundColor.a > 0)
		{
			april::rendersys->setBlendMode(april::BM_DEFAULT);
			april::rendersys->setColorMode(april::CM_DEFAULT);
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
		// text effect
		hstr colorCode = "";
		if (this->useEffectColor)
		{
			colorCode += this->effectColor.hex();
		}
		if (this->useEffectParameter)
		{
			colorCode += "," + this->effectParameter;
		}
		if (colorCode != "")
		{
			colorCode = ":" + colorCode;
		}
		if (this->effect == atres::TextEffect::Border)
		{
			text = "[b" + colorCode + "]" + text;
		}
		else if (this->effect == atres::TextEffect::Shadow)
		{
			text = "[s" + colorCode + "]" + text;
		}
		// strike-through
		if (this->strikeThrough)
		{
			colorCode = "";
			if (this->useStrikeThroughColor)
			{
				colorCode += this->strikeThroughColor.hex();
			}
			if (this->useStrikeThroughParameter)
			{
				colorCode += "," + this->strikeThroughParameter;
			}
			if (colorCode != "")
			{
				colorCode = ":" + colorCode;
			}
			text = "[t" + colorCode + "]" + text;
		}
		// strike-through
		if (this->underline)
		{
			colorCode = "";
			if (this->useUnderlineColor)
			{
				colorCode += this->underlineColor.hex();
			}
			if (this->useUnderlineParameter)
			{
				colorCode += "," + this->underlineParameter;
			}
			if (colorCode != "")
			{
				colorCode = ":" + colorCode;
			}
			text = "[u" + colorCode + "]" + text;
		}
		gvec2 offset = -this->textOffset;
		if (this->_autoScaleDirty)
		{
			this->_calcAutoScaleFont(this->font, rect, text, this->horzFormatting, this->vertFormatting);
			this->_autoScaleDirty = false;
		}
		hstr font = (this->autoScaledFont == "" ? this->font : this->autoScaledFont);
		atres::renderer->drawText(font, rect, text, this->horzFormatting, this->vertFormatting, color, offset);
	}

	hstr LabelBase::getProperty(chstr name)
	{
		if (name == "font")					return this->getFont();
		if (name == "text")					return this->getText();
		if (name == "text_key")				return this->getTextKey();
		if (name == "text_color")			return this->getTextColor().hex();
		if (name == "text_offset")			return april::gvec2ToHstr(this->getTextOffset());
		if (name == "text_offset_x")		return this->getTextOffsetX();
		if (name == "text_offset_y")		return this->getTextOffsetY();
		if (name == "min_auto_scale")		return this->getMinAutoScale();
		if (name == "auto_scaled_font")		return this->getAutoScaledFont();
		if (name == "horz_formatting")
		{
			if (this->horzFormatting == atres::Horizontal::Left)			return "left";
			if (this->horzFormatting == atres::Horizontal::Right)			return "right";
			if (this->horzFormatting == atres::Horizontal::Center)			return "center";
			if (this->horzFormatting == atres::Horizontal::LeftWrapped)		return "left_wrapped";
			if (this->horzFormatting == atres::Horizontal::RightWrapped)	return "right_wrapped";
			if (this->horzFormatting == atres::Horizontal::CenterWrapped)	return "center_wrapped";
			if (this->horzFormatting == atres::Horizontal::Justified)		return "justified";
		}
		if (name == "vert_formatting")
		{
			if (this->vertFormatting == atres::Vertical::Top)		return "top";
			if (this->vertFormatting == atres::Vertical::Center)	return "center";
			if (this->vertFormatting == atres::Vertical::Bottom)	return "bottom";
		}
		if (name == "effect")
		{
			hstr effect = "";
			if (this->effect == atres::TextEffect::None)	effect = "none";
			if (this->effect == atres::TextEffect::Border)	effect = "border";
			if (this->effect == atres::TextEffect::Shadow)	effect = "shadow";
			if (this->useEffectColor)
			{
				effect += ":" + this->effectColor.hex();
				if (this->useEffectParameter)
				{
					effect += "," + this->effectParameter;
				}
			}
			else if (this->useEffectParameter)
			{
				effect += ":," + this->effectParameter;
			}
			return effect;
		}
		if (name == "strike_through")		return this->isStrikeThrough();
		if (name == "underline")			return this->isUnderline();
		if (name == "background_color")		return this->getBackgroundColor().hex();
		if (name == "background_border")	return this->isBackgroundBorder();
		return "";
	}
	
	bool LabelBase::setProperty(chstr name, chstr value)
	{
		if (name == "font")						this->setFont(value);
		else if (name == "text_key")			this->setTextKey(value);
		else if (name == "text")				this->setText(value);
		else if (name == "text_color")			this->setTextColor(value);
		else if (name == "text_offset")			this->setTextOffset(april::hstrToGvec2(value));
		else if (name == "text_offset_x")		this->setTextOffsetX(value);
		else if (name == "text_offset_y")		this->setTextOffsetY(value);
		else if (name == "min_auto_scale")		this->setMinAutoScale(value);
		else if (name == "horz_formatting")
		{
			if (value == "left")				this->setHorzFormatting(atres::Horizontal::Left);
			else if (value == "right")			this->setHorzFormatting(atres::Horizontal::Right);
			else if (value == "center")			this->setHorzFormatting(atres::Horizontal::Center);
			else if (value == "left_wrapped")	this->setHorzFormatting(atres::Horizontal::LeftWrapped);
			else if (value == "right_wrapped")	this->setHorzFormatting(atres::Horizontal::RightWrapped);
			else if (value == "center_wrapped")	this->setHorzFormatting(atres::Horizontal::CenterWrapped);
			else if (value == "justified")		this->setHorzFormatting(atres::Horizontal::Justified);
			else
			{
				hlog::warn(logTag, "'horz_formatting=' does not support value '" + value + "'.");
				return false;
			}
		}
		else if (name == "vert_formatting")
		{
			if (value == "top")			this->setVertFormatting(atres::Vertical::Top);
			else if (value == "center")	this->setVertFormatting(atres::Vertical::Center);
			else if (value == "bottom")	this->setVertFormatting(atres::Vertical::Bottom);
			else
			{
				hlog::warn(logTag, "'vert_formatting=' does not support value '" + value + "'.");
				return false;
			}
		}
		else if (name == "effect")
		{
			this->setEffect(atres::TextEffect::None);
			this->setUseEffectColor(false);
			harray<hstr> values = value.split(":", 1, true);
			if (values.size() > 0)
			{
				if (values[0] == "none")		this->setEffect(atres::TextEffect::None);
				else if (values[0] == "shadow")	this->setEffect(atres::TextEffect::Shadow);
				else if (values[0] == "border")	this->setEffect(atres::TextEffect::Border);
				else
				{
					hlog::warn(logTag, "'effect=' does not support value '" + values[0] + "'.");
					return false;
				}
				if (values.size() > 1)
				{
					values = values[1].split(",", 1);
					if (values[0].isHex() && (values[0].size() == 6 || values[0].size() == 8))
					{
						this->setUseEffectColor(true);
						this->setEffectColor(values[0]);
					}
					else if (values[0] != "")
					{
						hlog::warn(logTag, "'effect=' is using invalid color modifier '" + values[0] + "'.");
						return false;
					}
					if (values.size() > 1)
					{
						this->setUseEffectParameter(true);
						this->setEffectParameter(values[1]);
					}
				}
			}
		}
		else if (name == "strike_through")		this->setStrikeThrough(value);
		else if (name == "underline")			this->setUnderline(value);
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
				this->_autoScaleDirty = true;
			}
		}
		else if (type == Event::TextChanged || type == Event::TextKeyChanged || type == Event::Resized)
		{
			this->_autoScaleDirty = true;
		}
	}
	
	void LabelBase::_calcAutoScaleFont(chstr fontName, grect rect, chstr text, atres::Horizontal horizontal, atres::Vertical vertical)
	{
		this->autoScaledFont = "";
		if (this->minAutoScale >= 1.0f || rect.w <= 0.0f || rect.h <= 0.0f)
		{
			return;
		}
		float autoScale = 1.0f;
		atres::Font* font = atres::renderer->getFont(fontName);
		if (font == NULL)
		{
			return;
		}
		// rendering changes the scale, this value has to be stored
		float fontScale = font->getScale() / font->getBaseScale();
		hstr realFontName = font->getName();
		bool needsScaling = false;
		gvec2 size;
		size.y = atres::renderer->getTextHeight(fontName, text, rect.w);
		// either one word that doesn't even fit in the initial rect-width or the height is too large for the rect
		if (size.y <= 0.0f || size.y > rect.h)
		{
			needsScaling = true;
		}
		if (!needsScaling && !horizontal.isWrapped())
		{
			size.x = atres::renderer->getTextWidth(fontName, text);
			if (size.x > 0.0f && size.x > rect.w)
			{
				needsScaling = true;
			}
		}
		if (needsScaling)
		{
			if (!horizontal.isWrapped())
			{
				size.x = atres::renderer->getTextWidth(fontName, text);
				float newY = atres::renderer->getTextHeight(fontName, text, size.x);
				int i = 0;
				while (size.y * SAFE_AUTO_SCALE_CHECK_VALUE < newY && i < MAX_AUTO_SCALE_STEPS)
				{
					size.x *= SAFE_AUTO_SCALE_FACTOR;
					newY = atres::renderer->getTextHeight(fontName, text, size.x);
					++i;
				}
				size.y = newY;
				autoScale = hmin(rect.w / size.x, rect.h / size.y);
			}
			else
			{
				float upperAutoScale = 1.0f;
				float lowerAutoScale = this->minAutoScale;
				float currentAutoScale = this->minAutoScale;
				autoScale = this->minAutoScale;
				for_iter (i, 0, MAX_AUTO_SCALE_STEPS)
				{
					currentAutoScale = lowerAutoScale + (upperAutoScale - lowerAutoScale) * 0.5f;
					size.y = atres::renderer->getTextHeight(realFontName + ":" + hstr(fontScale * currentAutoScale), text, rect.w);
					if (size.y > rect.h)
					{
						upperAutoScale = currentAutoScale;
					}
					else if (size.y < rect.h)
					{
						lowerAutoScale = autoScale = currentAutoScale;
					}
					else // however unlikely, the exact sweetspot was hit
					{
						autoScale = currentAutoScale;
						break;
					}
				}
			}
			this->autoScaledFont = realFontName + ":" + hstr(fontScale * hclamp(autoScale, this->minAutoScale, 1.0f));
		}
	}
	
}

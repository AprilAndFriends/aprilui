/// @file
/// @version 5.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/april.h>
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
	hmap<hstr, PropertyDescription> LabelBase::_propertyDescriptions;
	hmap<hstr, PropertyDescription::Accessor*> LabelBase::_getters;
	hmap<hstr, PropertyDescription::Accessor*> LabelBase::_setters;

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
		this->strikeThrough = other.strikeThrough;
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

	hmap<hstr, PropertyDescription>& LabelBase::getPropertyDescriptions() const
	{
		if (LabelBase::_propertyDescriptions.size() == 0)
		{
			LabelBase::_propertyDescriptions["font"] = PropertyDescription("font", PropertyDescription::Type::String);
			LabelBase::_propertyDescriptions["text_formatting"] = PropertyDescription("text_formatting", PropertyDescription::Type::Bool);
			LabelBase::_propertyDescriptions["text"] = PropertyDescription("text", PropertyDescription::Type::String);
			LabelBase::_propertyDescriptions["text_key"] = PropertyDescription("text_key", PropertyDescription::Type::String);
			LabelBase::_propertyDescriptions["text_color"] = PropertyDescription("text_color", PropertyDescription::Type::Color);
			LabelBase::_propertyDescriptions["text_offset"] = PropertyDescription("text_offset", PropertyDescription::Type::Gvec2f);
			LabelBase::_propertyDescriptions["text_offset_x"] = PropertyDescription("text_offset_x", PropertyDescription::Type::Float);
			LabelBase::_propertyDescriptions["text_offset_y"] = PropertyDescription("text_offset_y", PropertyDescription::Type::Float);
			LabelBase::_propertyDescriptions["min_auto_scale"] = PropertyDescription("min_auto_scale", PropertyDescription::Type::Float);
			LabelBase::_propertyDescriptions["auto_scaled_font"] = PropertyDescription("auto_scaled_font", PropertyDescription::Type::String);
			LabelBase::_propertyDescriptions["horz_formatting"] = PropertyDescription("horz_formatting", PropertyDescription::Type::Enum);
			LabelBase::_propertyDescriptions["vert_formatting"] = PropertyDescription("vert_formatting", PropertyDescription::Type::Enum);
			LabelBase::_propertyDescriptions["effect"] = PropertyDescription("effect", PropertyDescription::Type::Enum);
			LabelBase::_propertyDescriptions["strike_through"] = PropertyDescription("strike_through", PropertyDescription::Type::Bool);
			LabelBase::_propertyDescriptions["underline"] = PropertyDescription("underline", PropertyDescription::Type::Bool);
			LabelBase::_propertyDescriptions["background_color"] = PropertyDescription("background_color", PropertyDescription::Type::Color);
			LabelBase::_propertyDescriptions["background_border"] = PropertyDescription("background_border", PropertyDescription::Type::Bool);
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

	void LabelBase::setTextSymbolicColor(chstr value)
	{
		this->setTextColor(aprilui::_makeColor(value));
	}

	void LabelBase::setTextOffset(cgvec2f value)
	{
		if (this->textOffset != value)
		{
			this->textOffset = value;
			this->_autoScaleDirty = true;
		}
	}

	void LabelBase::setTextOffsetX(const float& value)
	{
		if (this->textOffset.x != value)
		{
			this->textOffset.x = value;
			this->_autoScaleDirty = true;
		}
	}

	void LabelBase::setTextOffsetY(const float& value)
	{
		if (this->textOffset.y != value)
		{
			this->textOffset.y = value;
			this->_autoScaleDirty = true;
		}
	}

	void LabelBase::setMinAutoScale(const float& value)
	{
		float newValue = hclamp(value, 0.1f, 1.0f);
		if (this->minAutoScale != newValue)
		{
			this->minAutoScale = newValue;
			this->_autoScaleDirty = true;
		}
	}

	void LabelBase::setEffectSymbolicColor(chstr value)
	{
		this->setEffectColor(aprilui::_makeColor(value));
	}

	void LabelBase::setBackgroundSymbolicColor(chstr value)
	{
		this->setBackgroundColor(aprilui::_makeColor(value));
	}

	void LabelBase::setStrikeThroughSymbolicColor(chstr value)
	{
		this->setStrikeThroughColor(aprilui::_makeColor(value));
	}

	void LabelBase::setUnderlineSymbolicColor(chstr value)
	{
		this->setUnderlineColor(aprilui::_makeColor(value));
	}

	void LabelBase::_calcAutoScaledFont(cgrectf rect)
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

	april::Color LabelBase::_makeBackgroundDrawColor(const april::Color& drawColor) const
	{
		return aprilui::_makeModifiedDrawColor(this->backgroundColor, drawColor);
	}

	void LabelBase::_drawLabelBackground(cgrectf rect, const april::Color& color, const april::Color& backgroundColor)
	{
		if (backgroundColor.a > 0)
		{
			april::rendersys->setBlendMode(april::BlendMode::Alpha);
			april::rendersys->setColorMode(april::ColorMode::Multiply);
			april::rendersys->drawFilledRect(rect, backgroundColor);
			if (this->backgroundBorder)
			{
				april::rendersys->drawRect(rect, april::Color(color, backgroundColor.a));
			}
		}
	}

	void LabelBase::_drawLabel(cgrectf rect, const april::Color& color)
	{
		if (this->text.size() == 0)
		{
			return;
		}
		april::Color drawColor = color * this->textColor;
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
		gvec2f offset = -this->textOffset;
		if (this->_autoScaleDirty)
		{
			this->_calcAutoScaleFont(this->font, rect, text, this->horzFormatting, this->vertFormatting);
			this->_autoScaleDirty = false;
		}
		hstr font = (this->autoScaledFont == "" ? this->font : this->autoScaledFont);
		atres::renderer->drawText(font, rect, text, this->horzFormatting, this->vertFormatting, drawColor, offset);
	}

	hstr LabelBase::getProperty(chstr name)
	{
		if (name == "horz_formatting")
		{
			if (this->horzFormatting == atres::Horizontal::Left)					return "left";
			if (this->horzFormatting == atres::Horizontal::Right)					return "right";
			if (this->horzFormatting == atres::Horizontal::Center)					return "center";
			if (this->horzFormatting == atres::Horizontal::LeftWrapped)				return "left_wrapped";
			if (this->horzFormatting == atres::Horizontal::LeftWrappedUntrimmed)	return "left_wrapped_untrimmed";
			if (this->horzFormatting == atres::Horizontal::RightWrapped)			return "right_wrapped";
			if (this->horzFormatting == atres::Horizontal::RightWrappedUntrimmed)	return "right_wrapped_untrimmed";
			if (this->horzFormatting == atres::Horizontal::CenterWrapped)			return "center_wrapped";
			if (this->horzFormatting == atres::Horizontal::CenterWrappedUntrimmed)	return "center_wrapped_untrimmed";
			if (this->horzFormatting == atres::Horizontal::Justified)				return "justified";
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
		if (name == "strike_through")
		{
			hstr strikeThrough = this->strikeThrough;
			if (this->useStrikeThroughColor)
			{
				strikeThrough += ":" + this->strikeThroughColor.hex();
				if (this->useStrikeThroughParameter)
				{
					strikeThrough += "," + this->strikeThroughParameter;
				}
			}
			else if (this->useStrikeThroughParameter)
			{
				strikeThrough += ":," + this->strikeThroughParameter;
			}
			return strikeThrough;
		}
		if (name == "underline")
		{
			hstr underline = this->underline;
			if (this->useUnderlineColor)
			{
				underline += ":" + this->underlineColor.hex();
				if (this->useUnderlineParameter)
				{
					underline += "," + this->underlineParameter;
				}
			}
			else if (this->useUnderlineParameter)
			{
				underline += ":," + this->underlineParameter;
			}
			return underline;
		}
		return "";
	}
	
	bool LabelBase::setProperty(chstr name, chstr value)
	{
		if (name == "horz_formatting")
		{
			if (value == "left")							this->setHorzFormatting(atres::Horizontal::Left);
			else if (value == "right")						this->setHorzFormatting(atres::Horizontal::Right);
			else if (value == "center")						this->setHorzFormatting(atres::Horizontal::Center);
			else if (value == "left_wrapped")				this->setHorzFormatting(atres::Horizontal::LeftWrapped);
			else if (value == "left_wrapped_untrimmed")		this->setHorzFormatting(atres::Horizontal::LeftWrappedUntrimmed);
			else if (value == "right_wrapped")				this->setHorzFormatting(atres::Horizontal::RightWrapped);
			else if (value == "right_wrapped_untrimmed")	this->setHorzFormatting(atres::Horizontal::RightWrappedUntrimmed);
			else if (value == "center_wrapped")				this->setHorzFormatting(atres::Horizontal::CenterWrapped);
			else if (value == "center_wrapped_untrimmed")	this->setHorzFormatting(atres::Horizontal::CenterWrappedUntrimmed);
			else if (value == "justified")					this->setHorzFormatting(atres::Horizontal::Justified);
			else
			{
				hlog::warn(logTag, "'horz_formatting=' does not support value '" + value + "'.");
				return false;
			}
			return true;
		}
		if (name == "vert_formatting")
		{
			if (value == "top")			this->setVertFormatting(atres::Vertical::Top);
			else if (value == "center")	this->setVertFormatting(atres::Vertical::Center);
			else if (value == "bottom")	this->setVertFormatting(atres::Vertical::Bottom);
			else
			{
				hlog::warn(logTag, "'vert_formatting=' does not support value '" + value + "'.");
				return false;
			}
			return true;
		}
		if (name == "effect")
		{
			this->setEffect(atres::TextEffect::None);
			this->setUseEffectColor(false);
			this->setUseEffectParameter(false);
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
					bool useColor = false;
					bool useParameter = false;
					april::Color color;
					hstr parameter;
					if (!this->_analyzeExtraParameters(name, values[1], useColor, useParameter, color, parameter))
					{
						return false;
					}
					this->setUseEffectColor(useColor);
					this->setUseEffectParameter(useParameter);
					this->setEffectColor(color);
					this->setEffectParameter(parameter);
				}
			}
			return true;
		}
		if (name == "strike_through")
		{
			this->setStrikeThrough(false);
			this->setUseStrikeThroughColor(false);
			this->setUseStrikeThroughParameter(false);
			harray<hstr> values = value.split(":", 1, true);
			if (values.size() > 0)
			{
				this->setStrikeThrough(values[0]);
				if (values.size() > 1)
				{
					bool useColor = false;
					bool useParameter = false;
					april::Color color;
					hstr parameter;
					if (!this->_analyzeExtraParameters(name, values[1], useColor, useParameter, color, parameter))
					{
						return false;
					}
					this->setUseStrikeThroughColor(useColor);
					this->setUseStrikeThroughParameter(useParameter);
					this->setStrikeThroughColor(color);
					this->setStrikeThroughParameter(parameter);
				}
			}
			return true;
		}
		if (name == "underline")
		{
			this->setUnderline(false);
			this->setUseUnderlineColor(false);
			this->setUseEffectParameter(false);
			harray<hstr> values = value.split(":", 1, true);
			if (values.size() > 0)
			{
				this->setUnderline(values[0]);
				if (values.size() > 1)
				{
					bool useColor = false;
					bool useParameter = false;
					april::Color color;
					hstr parameter;
					if (!this->_analyzeExtraParameters(name, values[1], useColor, useParameter, color, parameter))
					{
						return false;
					}
					this->setUseUnderlineColor(useColor);
					this->setUseUnderlineParameter(useParameter);
					this->setUnderlineColor(color);
					this->setUnderlineParameter(parameter);
				}
			}
			return true;
		}
		return false;
	}

	bool LabelBase::_analyzeExtraParameters(chstr name, chstr value, bool& useColor, bool& useParameter, april::Color& color, hstr& parameter) const
	{
		harray<hstr> values = value.split(",", 1);
		useColor = false;
		if (values[0] != "")
		{
			try
			{
				color = aprilui::_makeColor(values[0]);
			}
			catch (hexception&)
			{
				hlog::warnf(logTag, "'%s=' is using invalid color modifier '%s'.", name.cStr(), values[0].cStr());
				return false;
			}
			useColor = true;
		}
		useParameter = false;
		if (values.size() > 1)
		{
			parameter = values[1];
			useParameter = true;
		}
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
	
	void LabelBase::_calcAutoScaleFont(chstr fontName, cgrectf rect, chstr text, atres::Horizontal horizontal, atres::Vertical vertical)
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
		gvec2f size;
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

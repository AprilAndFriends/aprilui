/// @file
/// @version 4.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines a base class for objects with text.

#ifndef APRILUI_LABEL_BASE_H
#define APRILUI_LABEL_BASE_H

#include <april/Color.h>
#include <april/Keys.h>
#include <april/RenderSystem.h>
#include <atres/atres.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/harray.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "Event.h"
#include "PropertyDescription.h"

namespace aprilui
{
	class Dataset;

	class apriluiExport LabelBase
	{
		APRILUI_CLONEABLE_ABSTRACT(LabelBase)
	public:
		LabelBase();
		virtual ~LabelBase();

		virtual harray<PropertyDescription> getPropertyDescriptions() const;

		HL_DEFINE_GET(hstr, text, Text);
		virtual void setText(chstr value);
		HL_DEFINE_GET(hstr, textKey, TextKey);
		void setTextKey(chstr value);
		HL_DEFINE_GET(hstr, font, Font);
		void setFont(chstr value);
		HL_DEFINE_ISSET(textFormatting, TextFormatting);
		HL_DEFINE_GETSET(april::Color, textColor, TextColor);
		void setTextSymbolicColor(chstr value);
		HL_DEFINE_GET(gvec2, textOffset, TextOffset);
		void setTextOffset(cgvec2 value);
		HL_DEFINE_GET(float, textOffset.x, TextOffsetX);
		void setTextOffsetX(float value);
		HL_DEFINE_GET(float, textOffset.y, TextOffsetY);
		void setTextOffsetY(float value);
		HL_DEFINE_GET(float, minAutoScale, MinAutoScale);
		virtual void setMinAutoScale(float value);
		HL_DEFINE_GET(atres::Horizontal, horzFormatting, HorzFormatting);
		void setHorzFormatting(atres::Horizontal value);
		HL_DEFINE_GET(atres::Vertical, vertFormatting, VertFormatting);
		void setVertFormatting(atres::Vertical value);
		HL_DEFINE_GETSET(atres::TextEffect, effect, Effect);
		HL_DEFINE_ISSET(useEffectColor, UseEffectColor);
		HL_DEFINE_ISSET(useEffectParameter, UseEffectParameter);
		HL_DEFINE_GETSET(april::Color, effectColor, EffectColor);
		void setEffectSymbolicColor(chstr value);
		HL_DEFINE_GETSET(hstr, effectParameter, EffectParameter);
		HL_DEFINE_GETSET(april::Color, backgroundColor, BackgroundColor);
		void setBackgroundSymbolicColor(chstr value);
		HL_DEFINE_ISSET(strikeThrough, StrikeThrough);
		HL_DEFINE_ISSET(useStrikeThroughColor, UseStrikeThroughColor);
		HL_DEFINE_ISSET(useStrikeThroughParameter, UseStrikeThroughParameter);
		HL_DEFINE_GETSET(april::Color, strikeThroughColor, StrikeThroughColor);
		void setStrikeThroughSymbolicColor(chstr value);
		HL_DEFINE_GETSET(hstr, strikeThroughParameter, StrikeThroughParameter);
		HL_DEFINE_ISSET(underline, Underline);
		HL_DEFINE_ISSET(useUnderlineColor, UseUnderlineColor);
		HL_DEFINE_ISSET(useUnderlineParameter, UseUnderlineParameter);
		HL_DEFINE_GETSET(april::Color, underlineColor, UnderlineColor);
		void setUnderlineSymbolicColor(chstr value);
		HL_DEFINE_GETSET(hstr, underlineParameter, UnderlineParameter);
		HL_DEFINE_ISSET(backgroundBorder, BackgroundBorder);

		virtual Dataset* getDataset() const = 0;
		virtual hstr getAutoScaledFont() = 0;

		bool trySetTextKey(chstr textKey);
		
		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);

		virtual void notifyEvent(chstr type, EventArgs* args);

		virtual bool triggerEvent(chstr type, april::Key keyCode) = 0;
		virtual bool triggerEvent(chstr type, april::Key keyCode, chstr string) = 0;
		virtual bool triggerEvent(chstr type, april::Key keyCode, cgvec2 position, chstr string = "", void* userData = NULL) = 0;
		virtual bool triggerEvent(chstr type, april::Button buttonCode, chstr string = "", void* userData = NULL) = 0;
		virtual bool triggerEvent(chstr type, chstr string, void* userData = NULL) = 0;
		virtual bool triggerEvent(chstr type, void* userData = NULL) = 0;

		static float defaultMinAutoScale;

	protected:
		hstr text;
		hstr textKey;
		hstr font;
		bool textFormatting;
		april::Color textColor;
		gvec2 textOffset;
		float minAutoScale;
		hstr autoScaledFont;
		atres::Horizontal horzFormatting;
		atres::Vertical vertFormatting;
		atres::TextEffect effect;
		bool useEffectColor;
		bool useEffectParameter;
		april::Color effectColor;
		hstr effectParameter;
		bool strikeThrough;
		bool useStrikeThroughColor;
		bool useStrikeThroughParameter;
		april::Color strikeThroughColor;
		hstr strikeThroughParameter;
		bool underline;
		bool useUnderlineColor;
		bool useUnderlineParameter;
		april::Color underlineColor;
		hstr underlineParameter;
		april::Color backgroundColor;
		bool backgroundBorder;

		void _calcAutoScaledFont(cgrect rect);
		april::Color _makeBackgroundDrawColor(const april::Color& drawColor) const;
		bool _analyzeExtraParameters(chstr name, chstr value, bool& useColor, bool& useParameter, april::Color& color, hstr& parameter) const;
		
		void _drawLabelBackground(cgrect rect, const april::Color& color, const april::Color& backgroundColor);
		void _drawLabel(cgrect rect, const april::Color& color);

	private:
		bool _autoScaleDirty;

		void _calcAutoScaleFont(chstr fontName, cgrect rect, chstr text, atres::Horizontal horizontal, atres::Vertical vertical);

		static harray<PropertyDescription> _propertyDescriptions;

	};
	
}

#endif

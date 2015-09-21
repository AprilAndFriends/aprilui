/// @file
/// @version 4.1
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

		HL_DEFINE_GET(hstr, text, Text);
		virtual void setText(chstr value);
		HL_DEFINE_GET(hstr, textKey, TextKey);
		void setTextKey(chstr value);
		HL_DEFINE_GETSET(hstr, font, Font);
		HL_DEFINE_GETSET(april::Color, textColor, TextColor);
		HL_DEFINE_GETSET(gvec2, textOffset, TextOffset);
		HL_DEFINE_GETSET(float, textOffset.x, TextOffsetX);
		HL_DEFINE_GETSET(float, textOffset.y, TextOffsetY);
		HL_DEFINE_GETSET(atres::Horizontal, horzFormatting, HorzFormatting);
		HL_DEFINE_GETSET(atres::Vertical, vertFormatting, VertFormatting);
		HL_DEFINE_GETSET(atres::TextEffect, effect, Effect);
		HL_DEFINE_ISSET(useEffectColor, UseEffectColor);
		HL_DEFINE_ISSET(useEffectParameter, UseEffectParameter);
		HL_DEFINE_GETSET(april::Color, effectColor, EffectColor);
		HL_DEFINE_GETSET(hstr, effectParameter, EffectParameter);
		HL_DEFINE_GETSET(april::Color, backgroundColor, BackgroundColor);
		HL_DEFINE_ISSET(backgroundBorder, BackgroundBorder);

		virtual Dataset* getDataset() = 0;

		virtual harray<PropertyDescription> getPropertyDescriptions();

		bool trySetTextKey(chstr textKey);
		
		virtual void notifyEvent(chstr type, EventArgs* args);
		virtual bool triggerEvent(chstr type, april::Key keyCode) = 0;
		virtual bool triggerEvent(chstr type, april::Key keyCode, chstr string) = 0;
		virtual bool triggerEvent(chstr type, april::Key keyCode, gvec2 position, chstr string = "", void* userData = NULL) = 0;
		virtual bool triggerEvent(chstr type, april::Button buttonCode, chstr string, void* userData = NULL) = 0;
		virtual bool triggerEvent(chstr type, chstr string, void* userData = NULL) = 0;
		virtual bool triggerEvent(chstr type, void* userData = NULL) = 0;

		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);

		DEPRECATED_ATTRIBUTE atres::TextEffect getFontEffect() { return this->getEffect(); }
		DEPRECATED_ATTRIBUTE void setFontEffect(atres::TextEffect value) { this->setEffect(value); }
		
	protected:
		hstr text;
		hstr textKey;
		hstr font;
		bool textFormatting;
		april::Color textColor;
		gvec2 textOffset;
		atres::Horizontal horzFormatting;
		atres::Vertical vertFormatting;
		atres::TextEffect effect;
		bool useEffectColor;
		bool useEffectParameter;
		april::Color effectColor;
		hstr effectParameter;
		april::Color backgroundColor;
		bool backgroundBorder;

		april::Color _makeBackgroundDrawColor(april::Color drawColor);
		
		void _drawLabelBackground(grect rect, april::Color color, april::Color backgroundColor);
		void _drawLabel(grect rect, april::Color color);
		
	private:
		static harray<PropertyDescription> _propertyDescriptions;

	};
	
}

#endif

/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 3.11
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

namespace aprilui
{
	class Dataset;

	class apriluiExport LabelBase
	{
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
		HL_INLINE float getTextOffsetX() { return this->textOffset.x; }
		HL_INLINE void setTextOffsetX(float value) { this->textOffset.x = value; }
		HL_INLINE float getTextOffsetY() { return this->textOffset.y; }
		HL_INLINE void setTextOffsetY(float value) { this->textOffset.y = value; }
		HL_DEFINE_GETSET(atres::Alignment, horzFormatting, HorzFormatting);
		HL_DEFINE_GETSET(atres::Alignment, vertFormatting, VertFormatting);
		HL_DEFINE_GETSET(atres::Effect, fontEffect, FontEffect);
		HL_DEFINE_ISSET(useFontEffectColor, UseFontEffectColor);
		HL_DEFINE_GETSET(april::Color, fontEffectColor, FontEffectColor);
		HL_DEFINE_GETSET(april::Color, backgroundColor, BackgroundColor);

		virtual Dataset* getDataset() = 0;

		bool trySetTextKey(chstr textKey);
		
		virtual void notifyEvent(chstr name, void* params);
		// TODO - this needs to be seriously refactored
		virtual bool triggerEvent(chstr name, april::Key keyCode = april::AK_NONE, chstr extra = "") = 0;
		// TODO - this needs to be seriously refactored
		virtual bool triggerEvent(chstr name, april::Button buttonCode, chstr extra = "") = 0;
		// TODO - this needs to be seriously refactored
		virtual bool triggerEvent(chstr name, float x, float y, april::Key keyCode = april::AK_NONE, chstr extra = "") = 0;

		hstr getProperty(chstr name, bool* propertyExists = NULL);
		bool setProperty(chstr name, chstr value);
		
	protected:
		hstr text;
		hstr textKey;
		hstr font;
		bool textFormatting;
		april::Color textColor;
		gvec2 textOffset;
		atres::Alignment horzFormatting;
		atres::Alignment vertFormatting;
		atres::Effect fontEffect;
		bool useFontEffectColor;
		april::Color fontEffectColor;
		april::Color backgroundColor;
		
		void _drawLabel(grect rect, april::Color color, april::Color backgroundColor);
		
	};
	
}

#endif

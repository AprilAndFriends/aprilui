/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.52
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Defines a base class for objects with text.

#ifndef APRILUI_LABEL_BASE_H
#define APRILUI_LABEL_BASE_H

#include <april/Color.h>
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
		
		virtual Dataset* getDataset() = 0;
		hstr getText() { return this->mText; }
		virtual void setText(chstr value);
		hstr getTextKey() { return this->mTextKey; }
		void setTextKey(chstr value);
		hstr getFont() { return this->mFontName; }
		void setFont(chstr value) { this->mFontName = value; }

		bool trySetTextKey(chstr textKey);
		
		april::Color getTextColor() { return this->mTextColor; }
		void setTextColor(april::Color value) { this->mTextColor = value; }
		void setTextColor(chstr value) { this->mTextColor.set(value); }
		gvec2 getDrawOffset() { return this->mDrawOffset; }
		void setDrawOffset(gvec2 value) { this->mDrawOffset = value; }
		atres::Alignment getHorzFormatting() { return this->mHorzFormatting; }
		void setHorzFormatting(atres::Alignment value) { this->mHorzFormatting = value; }
		atres::Alignment getVertFormatting() { return this->mVertFormatting; }
		void setVertFormatting(atres::Alignment value) { this->mVertFormatting = value; }
		atres::Effect getFontEffect() { return this->mFontEffect; }
		void setFontEffect(atres::Effect value) { this->mFontEffect = value; }
		bool isUseFontEffectColor() { return this->mUseFontEffectColor; }
		void setUseFontEffectColor(bool value) { this->mUseFontEffectColor = value; }
		april::Color getFontEffectColor() { return this->mFontEffectColor; }
		void setFontEffectColor(april::Color value) { this->mFontEffectColor = value; }
		void setFontEffectColor(chstr value) { this->mFontEffectColor.set(value); }
		april::Color getBackgroundColor() { return this->mBackgroundColor; }
		void setBackgroundColor(april::Color value) { this->mBackgroundColor = value; }
		void setBackgroundColor(chstr value) { this->mBackgroundColor.set(value); }
		
		virtual void notifyEvent(chstr name, void* params);
		// TODO - this needs to be seriously refactored
		virtual bool triggerEvent(chstr name, unsigned int keyCode = 0, chstr extra = "") = 0;
		// TODO - this needs to be seriously refactored
		virtual bool triggerEvent(chstr name, float x, float y, unsigned int keyCode = 0, chstr extra = "") = 0;
		hstr getProperty(chstr name, bool* propertyExists = NULL);
		bool setProperty(chstr name, chstr value);
		
		DEPRECATED_ATTRIBUTE bool isUseBackground() { return (this->mBackgroundColor.a != 0); }
		DEPRECATED_ATTRIBUTE void setUseBackground(bool value) { this->mBackgroundColor.a = (value ? 128 : 0); }

	protected:
		hstr mText;
		hstr mTextKey;
		hstr mFontName;
		bool mTextFormatting;
		april::Color mTextColor;
		gvec2 mDrawOffset;
		atres::Alignment mHorzFormatting;
		atres::Alignment mVertFormatting;
		atres::Effect mFontEffect;
		bool mUseFontEffectColor;
		april::Color mFontEffectColor;
		april::Color mBackgroundColor;
		
		void _drawLabel(grect rect, april::Color color, april::Color backgroundColor);
		
	};
	
}

#endif

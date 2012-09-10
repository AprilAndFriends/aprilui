/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.01
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
		
		virtual void notifyEvent(chstr name, void* params);
		hstr getProperty(chstr name, bool* property_exists = NULL);
		bool setProperty(chstr name, chstr value);
		
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
		
		void _drawLabel(grect rect, april::Color color);
		
	};
	
}

#endif

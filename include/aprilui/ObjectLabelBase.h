/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.75
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
		
		virtual hstr getName() = 0;
		hstr getText() { return mText; }
		virtual void setText(chstr value);
		hstr getTextKey() { return mTextKey; }
		virtual void setTextKey(chstr value) { mTextKey = value; }
		
		hstr getFont() { return mFontName; }
		void setFont(chstr value) { mFontName = value; }
		
		gvec2 getDrawOffset() { return mDrawOffset; }
		void setDrawOffset(gvec2 value) { mDrawOffset = value; }
		atres::Alignment getHorzFormatting() { return mHorzFormatting; }
		void setHorzFormatting(atres::Alignment value) { mHorzFormatting = value; }
		atres::Alignment getVertFormatting() { return mVertFormatting; }
		void setVertFormatting(atres::Alignment value) { mVertFormatting = value; }
		atres::Effect getFontEffect() { return mFontEffect; }
		void setFontEffect(atres::Effect value) { mFontEffect = value; }
		april::Color getTextColor() { return mTextColor; }
		void setTextColor(april::Color value) { mTextColor = value; }
		void setTextColor(chstr value) { mTextColor.set(value); }
		
		hstr getProperty(chstr name, bool* property_exists = NULL);
		bool setProperty(chstr name, chstr value);
		
	protected:
		hstr mText;
		hstr mTextKey;
		hstr mFontName;
		bool mTextFormatting;
		gvec2 mDrawOffset;
		atres::Alignment mHorzFormatting;
		atres::Alignment mVertFormatting;
		atres::Effect mFontEffect;
		april::Color mTextColor;
		
		virtual hstr _getTextEntry(chstr key) = 0;
		virtual bool _hasTextKey(chstr key) = 0;

		void _drawLabel(grect rect, april::Color color);
		hstr _parseTextKey(chstr key);
		bool _processArgs(chstr argString, harray<hstr>& args);
		bool _preprocessFormat(chstr format, harray<hstr> args, hstr& preprocessedFormat, harray<hstr>& preprocessedArgs);
		bool _processFormat(chstr format, harray<hstr> args, hstr& result);
		/// @note The returned indexes count the positions relative to the last format tag (minus the 2 characters of the format tag itself), not from the beginning of the string
		bool _getFormatIndexes(chstr format, harray<int>& indexes);
		harray<hstr> _getTextEntries(harray<hstr> keys);
		
	};
	
}

#endif

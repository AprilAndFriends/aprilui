/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_LABEL_BASE_H
#define APRILUI_LABEL_BASE_H

#include <april/RenderSystem.h>
#include <atres/atres.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"

namespace aprilui
{
	class apriluiExport LabelBase
	{
	public:
		LabelBase();
		
		virtual float getAngle() = 0;
		hstr getText() { return mText; }
		virtual void setText(chstr value) { mText = value; }
		virtual void setTextKey(chstr key) = 0;
		
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
		
		virtual void setProperty(chstr name, chstr value);
		
	protected:
		hstr mText;
		hstr mFontName;
		bool mTextFormatting;
		gvec2 mDrawOffset;
		atres::Alignment mHorzFormatting;
		atres::Alignment mVertFormatting;
		atres::Effect mFontEffect;
		april::Color mTextColor;
		
		void _drawLabel(grect rect, unsigned char alpha);
		
	};
	
}

#endif

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
#include <hltypes/harray.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"

namespace aprilui
{
	class apriluiExport LabelBase
	{
	public:
		LabelBase(chstr name);
		
		virtual Dataset* getDataset() = 0;
		hstr getText() { return mText; }
		void setText(chstr value);
		hstr getTextKey() { return mTextKey; }
		void setTextKey(chstr value);
		
		hstr getFont() { return mFontName; }
		void setFont(chstr value) { mFontName = value; }
		
		gvec2 getDrawOffset() { return mDrawOffset; }
		void setDrawOffset(gvec2 value) { mDrawOffset = value; }
		atres::Horizontal getHorzFormatting() { return mHorzFormatting; }
		void setHorzFormatting(atres::Horizontal value) { mHorzFormatting = value; }
		atres::Vertical getVertFormatting() { return mVertFormatting; }
		void setVertFormatting(atres::Vertical value) { mVertFormatting = value; }
		atres::TextEffect getFontEffect() { return mFontEffect; }
		void setFontEffect(atres::TextEffect value) { mFontEffect = value; }
		april::Color getTextColor() { return mTextColor; }
		void setTextColor(april::Color value) { mTextColor = value; }
		void setTextColor(chstr value) { mTextColor.set(value); }
		
		virtual void notifyEvent(chstr name, void* params);
		virtual bool setProperty(chstr name, chstr value);
		
	protected:
		hstr mText;
		hstr mTextKey;
		hstr mFontName;
		bool mTextFormatting;
		gvec2 mDrawOffset;
		atres::Horizontal mHorzFormatting;
		atres::Vertical mVertFormatting;
		atres::TextEffect mFontEffect;
		april::Color mTextColor;
		
		void _drawLabel(grect rect, float alpha);
		
	};
	
}

#endif

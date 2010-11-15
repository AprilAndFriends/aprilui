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
#include <atres/Atres.h>
#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "AprilUIExport.h"

namespace AprilUI
{
	class AprilUIExport LabelBase
	{
	public:
		LabelBase(chstr name);
		
		hstr getText() { return mText; }
		virtual void setText(chstr value) { mText = value; }
		virtual void setTextKey(chstr key) = 0;
		
		hstr getFont() { return mFontName; }
		void setFont(chstr value) { mFontName = value; }
		
		float getDrawOffsetX() { return mDrawOffsetX; }
		void setDrawOffsetX(float value) { mDrawOffsetX = value; }
		float getDrawOffsetY() { return mDrawOffsetY; }
		void setDrawOffsetY(float value) { mDrawOffsetY = value; }
		Atres::Alignment getHorzFormatting() { return mHorzFormatting; }
		void setHorzFormatting(Atres::Alignment value) { mHorzFormatting = value; }
		Atres::Alignment getVertFormatting() { return mVertFormatting; }
		void setVertFormatting(Atres::Alignment value) { mVertFormatting = value; }
		Atres::Effect getFontEffect() { return mFontEffect; }
		void setFontEffect(Atres::Effect value) { mFontEffect = value; }
		April::Color getTextColor() { return mTextColor; }
		void setTextColor(April::Color value) { mTextColor = value; }
		void setTextColor(chstr value) { mTextColor.setColor(value); }
		
		virtual void setProperty(chstr name, chstr value);
		
	protected:
		hstr mText;
		hstr mFontName;
		bool mTextFormatting;
		float mDrawOffsetY;
		float mDrawOffsetX;
		Atres::Alignment mHorzFormatting;
		Atres::Alignment mVertFormatting;
		Atres::Effect mFontEffect;
		April::Color mTextColor;
		
		void _drawLabel(grect rect, float alpha);
		
	};
	
}

#endif

/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_LABEL_BASE_H
#define APRILUI_LABEL_BASE_H

#include <april/RenderSystem.h>
#include <atres/Atres.h>
#include <hltypes/hstring.h>

#include "AprilUIExport.h"

namespace AprilUI
{
	class AprilUIExport LabelBase
	{
	protected:
		hstr mText;
		hstr mFontName;
		bool mTextFormatting;
		Atres::Alignment mHorzFormatting;
		Atres::Alignment mVertFormatting;
		Atres::Effect mFontEffect;
		April::Color mTextColor;

		void _drawLabel(float offset_x,float offset_y,float width,float height,float alpha);
		
	public:
		LabelBase(chstr name);
		
		hstr getText() { return mText; }
		virtual void setText(chstr text) { mText=text; }
		virtual void setTextKey(chstr key) = 0;
		
		hstr getFont() { return mFontName; }
		void setFont(chstr font) { mFontName=font; }
		
		void setHorzFormatting(Atres::Alignment f) { mHorzFormatting=f; }
		Atres::Alignment getHorzFormatting() { return mHorzFormatting; }
		
		void setFontEffect(Atres::Effect f) { mFontEffect=f; }
		Atres::Effect getFontEffect() { return mFontEffect; }
		
		void setVertFormatting(Atres::Alignment f) { mVertFormatting=f; }
		Atres::Alignment getVertFormatting() { return mVertFormatting; }
		
		virtual void setProperty(chstr name,chstr value);

		void setTextColor(April::Color color) { mTextColor=color; }
		void setTextColor(chstr hex);
		April::Color getTextColor() { return mTextColor; }
	};
	
}

#endif

/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <atres/Atres.h>
#include <gtypes/Rectangle.h>
#include <hltypes/exception.h>
#include <hltypes/hstring.h>

#include "AprilUI.h"
#include "Exception.h"
#include "ObjectLabelBase.h"

namespace AprilUI
{
	LabelBase::LabelBase(chstr name) :
		   mTextColor(255,255,255,255)
	{
		mHorzFormatting=Atres::CENTER_WRAPPED;
		mVertFormatting=Atres::CENTER;
		mFontEffect=Atres::NONE;
		mTextFormatting=true;
		mText="LabelBase: "+name;
	}
	
	void LabelBase::_drawLabel(float offset_x,float offset_y,float width,float height,float alpha)
	{
		Atres::Font* font;
		try   { font=Atres::getFont(mFontName); }
		catch (hltypes::_resource_error e)
		{
			throw e;
		}
#ifdef _DEBUG
		if (AprilUI::isDebugMode())
		{
			April::rendersys->drawColoredQuad(offset_x, offset_y, width, height, 0, 0, 0, 0.5f);
		}
#endif
		if (mText.size() == 0)
		{
			return;
		}
		
		hstr text=mText;
		switch (mFontEffect)
		{
		case Atres::BORDER:
			text="[b]"+text;
			break;
		case Atres::SHADOW:
			text="[s]"+text;
			break;
		}
		April::Color color(mTextColor);
		color.a *= alpha;
		grect rect(offset_x,offset_y,width,height);
		try
		{
			if (mTextFormatting)
			{
				Atres::drawText(mFontName,rect,text,mHorzFormatting,mVertFormatting,color);
			}
			else
			{
				Atres::drawTextUnformatted(mFontName,rect,text,mHorzFormatting,mVertFormatting,color);
			}
		}
		catch (hltypes::_resource_error e)
		{ throw e; }
	}

	void LabelBase::setProperty(chstr name,chstr value)
	{
		if (name == "font") setFont(value);
		else if (name == "text") setText(value);
		else if (name == "wrap_text")
		{
			logMessage("\"wrap_text=\" is deprecated. Use \"horz_formatting=\" instead.");
		}
		else if (name == "horz_formatting")
		{
			if (value == "left")                setHorzFormatting(Atres::LEFT);
			else if (value == "right")          setHorzFormatting(Atres::RIGHT);
			else if (value == "center")         setHorzFormatting(Atres::CENTER);
			else if (value == "left_wrapped")   setHorzFormatting(Atres::LEFT_WRAPPED);
			else if (value == "right_wrapped")  setHorzFormatting(Atres::RIGHT_WRAPPED);
			else if (value == "center_wrapped") setHorzFormatting(Atres::CENTER_WRAPPED);
		}
		else if (name == "vert_formatting")
		{
			if (value == "top")         setVertFormatting(Atres::TOP);
			else if (value == "center") setVertFormatting(Atres::CENTER);
			else if (value == "bottom") setVertFormatting(Atres::BOTTOM);
		}
		else if (name == "color") setTextColor(value);
		else if (name == "effect")
		{
			if (value == "none")           setFontEffect(Atres::NONE);
			else if (value == "shadow")    setFontEffect(Atres::SHADOW);
			else if (value == "border")    setFontEffect(Atres::BORDER);
		}
	}

	void LabelBase::setTextColor(chstr hex)
	{
		mTextColor.setColor(hex);
	}
	
}

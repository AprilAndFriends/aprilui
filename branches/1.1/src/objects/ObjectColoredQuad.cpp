/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "ObjectColoredQuad.h"

namespace aprilui
{
	ColoredQuad::ColoredQuad(chstr name, grect rect) :
		Object("ColoredQuad", name, rect)
	{
	}

	void ColoredQuad::setColor(float a, float r, float g, float b)
	{
		mColor.set(COLOR_COMP_FOR_NEW_APRIL(r), 
				   COLOR_COMP_FOR_NEW_APRIL(g), 
				   COLOR_COMP_FOR_NEW_APRIL(b), 
				   COLOR_COMP_FOR_NEW_APRIL(a));
	}

	void ColoredQuad::OnDraw(gvec2 offset)
	{
		float alpha = getDerivedAlpha() * mColor.a_f();
		grect rect = mRect + offset;
		april::rendersys->drawColoredQuad(rect,
										  april::Color(mColor.r, mColor.g, mColor.b, COLOR_COMP_FOR_NEW_APRIL(alpha)));
	}

	void ColoredQuad::setProperty(chstr name, chstr value)
	{
		Object::setProperty(name, value);
		if (name == "r")
		{
			aprilui::logMessage("Attribute '" + name + "' is deprecated. Use 'color' instead");
			mColor.r = COLOR_COMP_FOR_NEW_APRIL(float(value));
		}
		else if (name == "g")
		{
			aprilui::logMessage("Attribute '" + name + "' is deprecated. Use 'color' instead");
			mColor.g = COLOR_COMP_FOR_NEW_APRIL(float(value));
		}
		else if (name == "b")
		{
			aprilui::logMessage("Attribute '" + name + "' is deprecated. Use 'color' instead");
			mColor.b = COLOR_COMP_FOR_NEW_APRIL(float(value));
		}
		else if (name == "a")
		{
			aprilui::logMessage("Attribute '" + name + "' is deprecated. Use 'color' instead");
			mColor.a = COLOR_COMP_FOR_NEW_APRIL(float(value));
		}
		else if (name == "color") mColor.set(HEXCOLOR_FIX_FOR_NEW_APRIL(value));
		else if (name == "color") mColor.set(HEXCOLOR_FIX_FOR_NEW_APRIL(value));
	}

	bool ColoredQuad::OnMouseDown(float x,float y,int button)
	{
		if (Object::OnMouseDown(x,y,button)) return true;
		if (isPointInside(x,y)) return true;
		return false;
	}

	bool ColoredQuad::OnMouseUp(float x,float y,int button)
	{
		if (Object::OnMouseUp(x,y,button)) return true;
		if (isPointInside(x,y))
		{
			triggerEvent("Click",x,y,0);
			return true;
		}
		return false;
	}
	
}

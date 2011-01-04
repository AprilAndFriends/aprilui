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
		mColor.setColor(a, r, g, b);
	}

	void ColoredQuad::OnDraw(gvec2 offset)
	{
		float alpha = getDerivedAlpha() * mColor.a_float();
		grect rect = mRect + offset;
		April::rendersys->drawColoredQuad(rect.x, rect.y, rect.w, rect.h,
			mColor.r_float(), mColor.g_float(), mColor.b_float(), alpha);
	}

	void ColoredQuad::setProperty(chstr name, chstr value)
	{
		Object::setProperty(name, value);
		if (name == "r")
		{
			aprilui::logMessage("Attribute '" + name + "' is deprecated. Use 'color' instead");
			mColor.r = (unsigned char)((float)value * 255);
		}
		else if (name == "g")
		{
			aprilui::logMessage("Attribute '" + name + "' is deprecated. Use 'color' instead");
			mColor.g = (unsigned char)((float)value * 255);
		}
		else if (name == "b")
		{
			aprilui::logMessage("Attribute '" + name + "' is deprecated. Use 'color' instead");
			mColor.b = (unsigned char)((float)value * 255);
		}
		else if (name == "a")
		{
			aprilui::logMessage("Attribute '" + name + "' is deprecated. Use 'color' instead");
			mColor.a = (unsigned char)((float)value * 255);
		}
		else if (name == "color") mColor.setColor(value);
		else if (name == "color") mColor.setColor(value);
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

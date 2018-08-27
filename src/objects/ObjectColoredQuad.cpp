/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "ObjectColoredQuad.h"

namespace aprilui
{
	ColoredQuad::ColoredQuad(chstr name, grectf rect) :
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

	void ColoredQuad::OnDraw(gvec2f offset)
	{
		float alpha = getDerivedAlpha() * mColor.a_f();
		april::rendersys->drawFilledRect(_getDrawRect() + offset, april::Color(mColor, COLOR_COMP_FOR_NEW_APRIL(alpha)));
	}

	bool ColoredQuad::setProperty(chstr name, chstr value)
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
		else if (name == "color") mColor.set(value);
		else if (name == "color") mColor.set(value);
		return 1;
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

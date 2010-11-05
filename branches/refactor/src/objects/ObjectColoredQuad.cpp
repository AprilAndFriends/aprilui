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

#include "ObjectColoredQuad.h"

namespace AprilUI
{
	ColoredQuad::ColoredQuad(chstr name,grect rect) :
		Object("ColoredQuad",name,rect)
	{
	}

	void ColoredQuad::setColor(float a,float r,float g,float b)
	{
		mColor.setColor(a,r,g,b);
	}

	void ColoredQuad::OnDraw(float offset_x,float offset_y)
	{
		float alpha=getDerivedAlpha()*mColor.a_float();
		April::rendersys->drawColoredQuad(mRect.x+offset_x, mRect.y+offset_y, mRect.w, mRect.h, mColor.r_float(), mColor.g_float(), mColor.b_float(), alpha);
	}

	void ColoredQuad::setProperty(chstr name,chstr value)
	{
		Object::setProperty(name,value);
		if      (name == "a") mColor.a=(unsigned char) ((float) value*255);
		else if (name == "r") mColor.r=(unsigned char) ((float) value*255);
		else if (name == "g") mColor.g=(unsigned char) ((float) value*255);
		else if (name == "b") mColor.b=(unsigned char) ((float) value*255);
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

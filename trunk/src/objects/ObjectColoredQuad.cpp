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
	
	void ColoredQuad::OnDraw()
	{
		april::Color color = mColor;
		if (mInheritsAlpha)
		{
			color.a = (unsigned char)(getDerivedAlpha() * color.a_f());
		}
		april::rendersys->drawColoredQuad(_getDrawRect(), color);
	}

	bool ColoredQuad::setProperty(chstr name, chstr value)
	{
		return Object::setProperty(name, value);
	}

	bool ColoredQuad::onMouseDown(float x, float y, int button)
	{
		if (Object::onMouseDown(x, y, button))
		{
			return true;
		}
		if (isCursorInside())
		{
			return true;
		}
		return false;
	}

	bool ColoredQuad::onMouseUp(float x, float y, int button)
	{
		if (Object::onMouseUp(x, y, button))
		{
			return true;
		}
		if (isCursorInside())
		{
			_triggerEvent("Click", x, y, button);
			return true;
		}
		return false;
	}
	
}

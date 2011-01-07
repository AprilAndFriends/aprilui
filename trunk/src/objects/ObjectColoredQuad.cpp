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
		VisualObject("ColoredQuad", name, rect)
	{
	}
	
	void ColoredQuad::OnDraw(gvec2 offset)
	{
		april::Color color = mColor;
		color.a = (unsigned char)(getDerivedAlpha() * color.a_f());
		// TODO - remove after implementing proper global rotation
		grect rect = mRect + offset;
		gmat4 originalMatrix = april::rendersys->getModelviewMatrix();
		april::rendersys->setIdentityTransform();
		april::rendersys->translate(rect.x + rect.w / 2, rect.y + rect.h / 2);
		april::rendersys->rotate(getAngle());
		april::rendersys->drawColoredQuad(grect(-rect.getSize() / 2, rect.getSize()), color);
		april::rendersys->setModelviewMatrix(originalMatrix);
	}

	void ColoredQuad::setProperty(chstr name, chstr value)
	{
		VisualObject::setProperty(name, value);
	}

	bool ColoredQuad::OnMouseDown(float x, float y, int button)
	{
		if (VisualObject::OnMouseDown(x, y, button))
		{
			return true;
		}
		if (isCursorInside())
		{
			return true;
		}
		return false;
	}

	bool ColoredQuad::OnMouseUp(float x, float y, int button)
	{
		if (VisualObject::OnMouseUp(x, y, button))
		{
			return true;
		}
		if (isCursorInside())
		{
			triggerEvent("Click", x, y, 0);
			return true;
		}
		return false;
	}
	
}

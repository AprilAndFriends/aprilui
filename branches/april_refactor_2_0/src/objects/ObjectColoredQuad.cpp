/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "ObjectColoredQuad.h"

namespace aprilui
{
	ColoredQuad::ColoredQuad(chstr name, grect rect) :
		Object(name, rect)
	{
	}
	
	ColoredQuad::~ColoredQuad()
	{
	}

	Object* ColoredQuad::createInstance(chstr name, grect rect)
	{
		return new ColoredQuad(name, rect);
	}

	void ColoredQuad::OnDraw()
	{
		april::rendersys->drawFilledRect(_getDrawRect(), _getDrawColor());
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

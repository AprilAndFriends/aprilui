/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.5
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
		april::rendersys->drawFilledRect(this->_getDrawRect(), this->_getDrawColor());
	}

	bool ColoredQuad::onMouseDown(int button)
	{
		if (Object::onMouseDown(button))
		{
			return true;
		}
		if (this->isCursorInside())
		{
			this->triggerEvent("MouseDown", button);
			return true;
		}
		return false;
	}

	bool ColoredQuad::onMouseUp(int button)
	{
		if (Object::onMouseUp(button))
		{
			return true;
		}
		if (this->isCursorInside())
		{
			this->triggerEvent("Click", button);
			return true;
		}
		return false;
	}
	
}

/// @file
/// @version 3.2
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
	ColoredQuad::ColoredQuad(chstr name, grect rect) : Object(name, rect)
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

	bool ColoredQuad::onMouseDown(april::Key keyCode)
	{
		if (Object::onMouseDown(keyCode))
		{
			return true;
		}
		if (this->isCursorInside())
		{
			this->triggerEvent("MouseDown", keyCode);
			return true;
		}
		return false;
	}

	bool ColoredQuad::onMouseUp(april::Key keyCode)
	{
		if (Object::onMouseUp(keyCode))
		{
			return true;
		}
		if (this->isCursorInside())
		{
			this->triggerEvent("Click", keyCode);
			return true;
		}
		return false;
	}
	
}

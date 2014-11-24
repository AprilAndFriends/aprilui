/// @file
/// @version 3.5
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
	ColoredQuad::ColoredQuad(chstr name) : Object(name)
	{
	}

	ColoredQuad::~ColoredQuad()
	{
	}

	Object* ColoredQuad::createInstance(chstr name)
	{
		return new ColoredQuad(name);
	}

	void ColoredQuad::_draw()
	{
		april::rendersys->drawFilledRect(this->_getDrawRect(), this->_getDrawColor());
	}
	
}

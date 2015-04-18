/// @file
/// @version 4.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "ObjectRect.h"

namespace aprilui
{
	Rect::Rect(chstr name) : Object(name)
	{
	}

	Rect::Rect(const Rect& other) : Object(other)
	{
	}

	Rect::~Rect()
	{
	}

	Object* Rect::createInstance(chstr name)
	{
		return new Rect(name);
	}

	void Rect::_draw()
	{
		float disabledAlphaFactor = this->_getDisabledAlphaFactor();
		april::Color drawColor = this->_getDrawColor();
		drawColor.a = (unsigned char)(drawColor.a * disabledAlphaFactor);
		april::rendersys->drawRect(this->_getDrawRect(), drawColor);
	}
	
}

/// @file
/// @version 5.1
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
	Rect::Rect(chstr name) :
		Object(name)
	{
	}

	Rect::Rect(const Rect& other) :
		Object(other)
	{
	}

	Object* Rect::createInstance(chstr name)
	{
		return new Rect(name);
	}

	void Rect::_draw()
	{
		april::rendersys->setBlendMode(april::BlendMode::Alpha);
		april::rendersys->setColorMode(april::ColorMode::Multiply);
		april::rendersys->drawRect(this->_makeDrawRect(), this->_makeDrawColor());
	}
	
}

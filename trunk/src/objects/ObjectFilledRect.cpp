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
#include "ObjectFilledRect.h"

namespace aprilui
{
	FilledRect::FilledRect(chstr name) : Object(name)
	{
	}

	FilledRect::FilledRect(const FilledRect& other) : Object(other)
	{
	}

	FilledRect::~FilledRect()
	{
	}

	Object* FilledRect::createInstance(chstr name)
	{
		return new FilledRect(name);
	}

	void FilledRect::_draw()
	{
		april::rendersys->drawFilledRect(this->_getDrawRect(), this->_getDrawColor());
	}
	
}
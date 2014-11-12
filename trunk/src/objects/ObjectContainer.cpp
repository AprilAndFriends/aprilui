/// @file
/// @version 3.5
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/Color.h>
#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "ObjectContainer.h"

namespace aprilui
{
	Container::Container(chstr name, grect rect) : Object(name, rect)
	{
		this->scrollArea = NULL;
		this->scrollBarH = NULL;
		this->scrollBarV = NULL;
		this->debugColor = april::Color(april::Color::Orange, 32);
	}

	Container::~Container()
	{
	}

	Object* Container::createInstance(chstr name, grect rect)
	{
		return new Container(name, rect);
	}

}

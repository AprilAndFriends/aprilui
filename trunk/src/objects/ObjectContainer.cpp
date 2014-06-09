/// @file
/// @version 3.2
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
	}

	Container::~Container()
	{
	}

	Object* Container::createInstance(chstr name, grect rect)
	{
		return new Container(name, rect);
	}

	void Container::OnDraw()
	{
		Object::OnDraw();
		if (aprilui::isDebugEnabled())
		{
			grect rect = this->_getDrawRect();
			april::rendersys->drawFilledRect(rect, april::Color(april::Color::Red, 32));
			april::rendersys->drawRect(rect, april::Color(april::Color::Yellow, 64));
		}
	}

}

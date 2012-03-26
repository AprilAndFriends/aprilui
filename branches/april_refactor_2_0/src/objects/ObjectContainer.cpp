/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.52
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <april/Color.h>
#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "ObjectContainer.h"

namespace aprilui
{
	Container::Container(chstr name, grect rect) :
		Object(name, rect)
	{
		mScrollArea = NULL;
		mScrollBarH = NULL;
		mScrollBarV = NULL;
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
		if (!aprilui::isDebugEnabled())
		{
			Object::OnDraw();
		}
		else
		{
			grect rect = _getDrawRect();
			april::rendersys->drawFilledRect(rect, april::Color(APRIL_COLOR_RED, 64));
			april::rendersys->drawRect(rect, april::Color(APRIL_COLOR_YELLOW, 64));
		}
	}

}

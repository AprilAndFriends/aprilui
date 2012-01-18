/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.5
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
		mClipping = false;
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
		if (aprilui::isDebugEnabled())
		{
			april::rendersys->drawColoredQuad(_getDrawRect(), april::Color(APRIL_COLOR_WHITE, 128));
		}
	}

	hstr Container::getProperty(chstr name, bool* property_exists)
	{
		if (property_exists != NULL)
		{
			*property_exists = true;
		}
		if (name == "clipping")	return getClipping();
		return Object::getProperty(name, property_exists);
	}

	bool Container::setProperty(chstr name, chstr value)
	{
		if (name == "clipping") setClipping(value);
		else return Object::setProperty(name, value);
		return true;
	}

}

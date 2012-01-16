/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.4
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
		mScrollMode = Disabled;
	}

	Container::~Container()
	{
	}

	Object* Container::createInstance(chstr name, grect rect)
	{
		return new Container(name, rect);
	}

	void Container::update(float k)
	{
		Object::update(k);
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
		if (name == "scroll_mode")
		{
			if (mScrollMode == Disabled)	return "disabled";
			if (mScrollMode == Invisible)	return "invisible";
			if (mScrollMode == ActiveOnly)	return "active_only";
			if (mScrollMode == Always)		return "always";
		}
		return Object::getProperty(name, property_exists);
	}

	bool Container::setProperty(chstr name, chstr value)
	{
		if (name == "clipping") setClipping(value);
		else if (name == "scroll_mode")
		{
			if (value == "disabled")	setScrollMode(Disabled);
			if (value == "invisible")	setScrollMode(Invisible);
			if (value == "active_only")	setScrollMode(ActiveOnly);
			if (value == "always")		setScrollMode(Always);
		}
		else return Object::setProperty(name, value);
		return true;
	}

	bool Container::onMouseDown(float x, float y, int button)
	{
		if (Object::onMouseDown(x, y, button))
		{
			return true;
		}
		if (isCursorInside())
		{
            _triggerEvent("MouseDown", x, y, button);
			return true;
		}
		return false;
	}

	bool Container::onMouseUp(float x, float y, int button)
	{
		if (Object::onMouseUp(x, y, button))
		{
			return true;
		}
		if (isCursorInside())
		{
			_triggerEvent("Click", x, y, button);
			return true;
		}
		return false;
	}

	void Container::onMouseMove(float x, float y)
	{
		Object::onMouseMove(x, y);
	}
	
}

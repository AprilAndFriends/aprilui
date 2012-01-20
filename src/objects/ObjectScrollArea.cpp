/// @file
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
#include "ObjectScrollArea.h"

namespace aprilui
{
	ScrollArea::ScrollArea(chstr name, grect rect) :
		Object(name, rect)
	{
		mClip = true;
		mAllowTouch = false;
	}

	ScrollArea::~ScrollArea()
	{
	}

	Object* ScrollArea::createInstance(chstr name, grect rect)
	{
		return new ScrollArea(name, rect);
	}

	void ScrollArea::update(float k)
	{
		Object::update(k);
	}

	void ScrollArea::OnDraw()
	{
		Object::OnDraw();
	}

	void ScrollArea::notifyEvent(chstr name, void* params)
	{
		Object::notifyEvent(name, params);
		if (name == "AttachToObject")
		{
			Container* parent = dynamic_cast<Container*>(mParent);
			if (parent != NULL)
			{
				parent->_setScrollArea(this);
			}
		}
		else if (name == "DetachFromObject")
		{
			Container* parent = dynamic_cast<Container*>(mParent);
			if (parent != NULL)
			{
				parent->_setScrollArea(NULL);
			}
		}
	}

	hstr ScrollArea::getProperty(chstr name, bool* property_exists)
	{
		if (property_exists != NULL)
		{
			*property_exists = true;
		}
		if (name == "allow_touch")	return isAllowTouch();
		return Object::getProperty(name, property_exists);
	}

	bool ScrollArea::setProperty(chstr name, chstr value)
	{
		if (name == "allow_touch")	setAllowTouch(value);
		else return Object::setProperty(name, value);
		return true;
	}

	bool ScrollArea::onMouseDown(float x, float y, int button)
	{
		return Object::onMouseDown(x, y, button);
	}

	bool ScrollArea::onMouseUp(float x, float y, int button)
	{
		return Object::onMouseUp(x, y, button);
	}
	
	void ScrollArea::onMouseMove(float x, float y)
	{
		Object::onMouseMove(x, y);
	}
	
}

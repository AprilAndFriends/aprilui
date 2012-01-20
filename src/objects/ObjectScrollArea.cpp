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

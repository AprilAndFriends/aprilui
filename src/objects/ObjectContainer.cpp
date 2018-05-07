/// @file
/// @version 4.3
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
#include "ObjectScrollArea.h"

namespace aprilui
{
	Container::Container(chstr name) : Object(name)
	{
		this->scrollArea = NULL;
		this->scrollBarH = NULL;
		this->scrollBarV = NULL;
		this->debugColor = april::Color(april::Color::Red, 32);
	}

	Container::Container(const Container& other) : Object(other)
	{
		this->scrollArea = NULL;
		this->scrollBarH = NULL;
		this->scrollBarV = NULL;
		this->debugColor = other.debugColor;
	}

	Container::~Container()
	{
	}

	Object* Container::createInstance(chstr name)
	{
		return new Container(name);
	}

	void Container::notifyEvent(chstr type, EventArgs* args)
	{
		Object::notifyEvent(type, args);
		if (type == Event::Resized)
		{
			if (this->scrollArea != NULL)
			{
				this->scrollArea->snapScrollOffset();
				this->scrollArea->_updateOobChildren();
			}
		}
	}

}

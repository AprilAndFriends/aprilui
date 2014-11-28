/// @file
/// @version 4.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "CallbackEvent.h"
#include "EventArgs.h"
#include "ObjectScrollBar.h"
#include "ObjectScrollBarButtonBackground.h"

namespace aprilui
{
	ScrollBarButtonBackground::ScrollBarButtonBackground(chstr name) : ImageButton(name)
	{
		this->registerEvent(aprilui::Event::Click, new aprilui::CallbackEvent(&_click));
	}

	ScrollBarButtonBackground::ScrollBarButtonBackground(const ScrollBarButtonBackground& other) : ImageButton(other)
	{
	}

	ScrollBarButtonBackground::~ScrollBarButtonBackground()
	{
	}

	Object* ScrollBarButtonBackground::createInstance(chstr name)
	{
		return new ScrollBarButtonBackground(name);
	}

	void ScrollBarButtonBackground::notifyEvent(chstr type, EventArgs* args)
	{
		ImageButton::notifyEvent(type, args);
		if (type == Event::AttachedToObject)
		{
			ScrollBar* parent = dynamic_cast<ScrollBar*>(this->parent);
			if (parent != NULL)
			{
				parent->_setButtonBackground(this);
			}
		}
		else if (type == Event::DetachedFromObject)
		{
			ScrollBar* parent = dynamic_cast<ScrollBar*>(this->parent);
			if (parent != NULL)
			{
				parent->_unsetButtonBackground(this);
			}
		}
	}

	void ScrollBarButtonBackground::_click(EventArgs* args)
	{
		if (args->object != NULL)
		{
			ScrollBar* scrollBar = dynamic_cast<ScrollBar*>(args->object->getParent());
			if (scrollBar != NULL)
			{
				gvec2 position = args->object->transformToLocalSpace(aprilui::getCursorPosition());
				scrollBar->addScrollValue(scrollBar->_calcScrollJump(position.x, position.y, args->object->getSize()));
			}
		}
	}

}

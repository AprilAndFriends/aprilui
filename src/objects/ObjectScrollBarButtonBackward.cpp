/// @file
/// @version 4.3
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
#include "ObjectContainer.h"
#include "EventArgs.h"
#include "ObjectScrollBar.h"
#include "ObjectScrollBarButtonBackward.h"

namespace aprilui
{
	ScrollBarButtonBackward::ScrollBarButtonBackward(chstr name) : ImageButton(name)
	{
		this->registerEvent(aprilui::Event::Click, new aprilui::CallbackEvent(&_click));
	}

	ScrollBarButtonBackward::ScrollBarButtonBackward(const ScrollBarButtonBackward& other) : ImageButton(other)
	{
	}

	ScrollBarButtonBackward::~ScrollBarButtonBackward()
	{
	}

	Object* ScrollBarButtonBackward::createInstance(chstr name)
	{
		return new ScrollBarButtonBackward(name);
	}

	void ScrollBarButtonBackward::notifyEvent(chstr type, EventArgs* args)
	{
		ImageButton::notifyEvent(type, args);
		if (type == Event::AttachedToObject)
		{
			ScrollBar* parent = dynamic_cast<ScrollBar*>(this->parent);
			if (parent != NULL)
			{
				parent->_setButtonBackward(this);
			}
		}
		else if (type == Event::DetachedFromObject)
		{
			ScrollBar* parent = dynamic_cast<ScrollBar*>(this->parent);
			if (parent != NULL)
			{
				parent->_unsetButtonBackward(this);
			}
		}
	}

	void ScrollBarButtonBackward::_click(EventArgs* args)
	{
		if (args->baseObject != NULL)
		{
			ScrollBar* scrollBar = dynamic_cast<ScrollBar*>(args->baseObject->getParent());
			if (scrollBar != NULL)
			{
				scrollBar->addScrollValueBackward();
			}
		}
	}

}

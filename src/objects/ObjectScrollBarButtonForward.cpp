/// @file
/// @version 3.14
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "EventArgs.h"
#include "EventUtils.h"
#include "ObjectScrollBar.h"
#include "ObjectScrollBarButtonForward.h"

namespace aprilui
{
	ScrollBarButtonForward::ScrollBarButtonForward(chstr name, grect rect) : ImageButton(name, rect)
	{
		_SET_CLICK_EVENT_FUNCTION(this, _click);
	}

	ScrollBarButtonForward::~ScrollBarButtonForward()
	{
	}

	Object* ScrollBarButtonForward::createInstance(chstr name, grect rect)
	{
		return new ScrollBarButtonForward(name, rect);
	}

	void ScrollBarButtonForward::notifyEvent(chstr name, void* params)
	{
		ImageButton::notifyEvent(name, params);
		if (name == "AttachToObject")
		{
			ScrollBar* parent = dynamic_cast<ScrollBar*>(this->parent);
			if (parent != NULL)
			{
				parent->_setButtonForward(this);
			}
		}
		else if (name == "DetachFromObject")
		{
			ScrollBar* parent = dynamic_cast<ScrollBar*>(this->parent);
			if (parent != NULL)
			{
				parent->_unsetButtonForward(this);
			}
		}
	}

	void ScrollBarButtonForward::_click(EventArgs* args)
	{
		ScrollBar* scrollBar = dynamic_cast<ScrollBar*>(args->object->getParent());
		if (scrollBar != NULL)
		{
			scrollBar->addScrollValueForward();
		}
	}

}

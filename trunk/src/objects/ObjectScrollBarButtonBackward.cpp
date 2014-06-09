/// @file
/// @version 3.2
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
#include "ObjectScrollBarButtonBackward.h"

namespace aprilui
{
	ScrollBarButtonBackward::ScrollBarButtonBackward(chstr name, grect rect) : ImageButton(name, rect)
	{
		_SET_CLICK_EVENT_FUNCTION(this, _click);
	}

	ScrollBarButtonBackward::~ScrollBarButtonBackward()
	{
	}

	Object* ScrollBarButtonBackward::createInstance(chstr name, grect rect)
	{
		return new ScrollBarButtonBackward(name, rect);
	}

	void ScrollBarButtonBackward::notifyEvent(chstr name, void* params)
	{
		ImageButton::notifyEvent(name, params);
		if (name == "AttachToObject")
		{
			ScrollBar* parent = dynamic_cast<ScrollBar*>(this->parent);
			if (parent != NULL)
			{
				parent->_setButtonBackward(this);
			}
		}
		else if (name == "DetachFromObject")
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
		ScrollBar* scrollBar = dynamic_cast<ScrollBar*>(args->object->getParent());
		if (scrollBar != NULL)
		{
			scrollBar->addScrollValueBackward();
		}
	}

}

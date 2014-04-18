/// @file
/// @author  Boris Mikic
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
#include "ObjectScrollBarButtonBackground.h"

namespace aprilui
{
	ScrollBarButtonBackground::ScrollBarButtonBackground(chstr name, grect rect) : ImageButton(name, rect)
	{
		_SET_CLICK_EVENT_FUNCTION(this, _click);
	}

	ScrollBarButtonBackground::~ScrollBarButtonBackground()
	{
	}

	Object* ScrollBarButtonBackground::createInstance(chstr name, grect rect)
	{
		return new ScrollBarButtonBackground(name, rect);
	}

	void ScrollBarButtonBackground::notifyEvent(chstr name, void* params)
	{
		ImageButton::notifyEvent(name, params);
		if (name == "AttachToObject")
		{
			ScrollBar* parent = dynamic_cast<ScrollBar*>(this->parent);
			if (parent != NULL)
			{
				parent->_setButtonBackground(this);
			}
		}
		else if (name == "DetachFromObject")
		{
			ScrollBar* parent = dynamic_cast<ScrollBar*>(this->parent);
			if (parent != NULL)
			{
				parent->_setButtonBackground(NULL);
			}
		}
	}

	void ScrollBarButtonBackground::_click(EventArgs* args)
	{
		ScrollBar* scrollBar = dynamic_cast<ScrollBar*>(args->object->getParent());
		if (scrollBar != NULL)
		{
			gvec2 position = args->object->transformToLocalSpace(aprilui::getCursorPosition());
			scrollBar->addScrollValue(scrollBar->_calcScrollJump(position.x, position.y, args->object->getSize()));
		}
	}

}

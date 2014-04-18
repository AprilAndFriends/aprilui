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
#include "ObjectScrollBarButtonBackward.h"
#include "ObjectScrollBarButtonSlider.h"

namespace aprilui
{
	ScrollBarButtonSlider::ScrollBarButtonSlider(chstr name, grect rect) : ImageButton(name, rect)
	{
		_SET_MOUSEDOWN_EVENT_FUNCTION(this, _mouseDown);
		_SET_CLICK_EVENT_FUNCTION(this, _click);
	}

	ScrollBarButtonSlider::~ScrollBarButtonSlider()
	{
	}

	Object* ScrollBarButtonSlider::createInstance(chstr name, grect rect)
	{
		return new ScrollBarButtonSlider(name, rect);
	}

	void ScrollBarButtonSlider::notifyEvent(chstr name, void* params)
	{
		ImageButton::notifyEvent(name, params);
		if (name == "AttachToObject")
		{
			ScrollBar* parent = dynamic_cast<ScrollBar*>(this->parent);
			if (parent != NULL)
			{
				parent->_setButtonSlider(this);
			}
		}
		else if (name == "DetachFromObject")
		{
			ScrollBar* parent = dynamic_cast<ScrollBar*>(this->parent);
			if (parent != NULL)
			{
				parent->_setButtonSlider(NULL);
			}
		}
	}

	void ScrollBarButtonSlider::_mouseDown(EventArgs* args)
	{
		ScrollBar* scrollBar = dynamic_cast<ScrollBar*>(args->object->getParent());
		if (scrollBar != NULL)
		{
			scrollBar->_clickPosition = aprilui::getCursorPosition() / scrollBar->getDerivedScale() -
				scrollBar->buttonSlider->getPosition() + scrollBar->buttonBackward->getSize();
		}
	}

	void ScrollBarButtonSlider::_click(EventArgs* args)
	{
		ScrollBar* scrollBar = dynamic_cast<ScrollBar*>(args->object->getParent());
		if (scrollBar != NULL)
		{
			scrollBar->_initAreaDragging();
			scrollBar->_adjustDragSpeed();
		}
	}

}

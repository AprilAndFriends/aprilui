/// @file
/// @version 3.5
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
#include "ObjectScrollBarButtonSlider.h"

namespace aprilui
{
	ScrollBarButtonSlider::ScrollBarButtonSlider(chstr name) : ImageButton(name)
	{
		this->registerEvent(aprilui::Event::MouseDown, new aprilui::CallbackEvent(&_mouseDown));
		this->registerEvent(aprilui::Event::Click, new aprilui::CallbackEvent(&_click));
	}

	ScrollBarButtonSlider::~ScrollBarButtonSlider()
	{
	}

	Object* ScrollBarButtonSlider::createInstance(chstr name)
	{
		return new ScrollBarButtonSlider(name);
	}

	void ScrollBarButtonSlider::notifyEvent(chstr type, EventArgs* args)
	{
		ImageButton::notifyEvent(type, args);
		if (type == Event::AttachedToObject)
		{
			ScrollBar* parent = dynamic_cast<ScrollBar*>(this->parent);
			if (parent != NULL)
			{
				parent->_setButtonSlider(this);
			}
		}
		else if (type == Event::DetachedFromObject)
		{
			ScrollBar* parent = dynamic_cast<ScrollBar*>(this->parent);
			if (parent != NULL)
			{
				parent->_unsetButtonSlider(this);
			}
		}
	}

	void ScrollBarButtonSlider::_mouseDown(EventArgs* args)
	{
		if (args->object != NULL)
		{
			ScrollBar* scrollBar = dynamic_cast<ScrollBar*>(args->object->getParent());
			if (scrollBar != NULL)
			{
				ScrollBarButtonBackground* buttonBackground = scrollBar->_getButtonBackground();
				if (buttonBackground != NULL)
				{
					gvec2 position = args->object->transformToLocalSpace(aprilui::getCursorPosition());
					position = args->object->getDerivedPoint(position, scrollBar) - args->object->getPosition() + buttonBackground->getPosition();
					scrollBar->_clickPosition = buttonBackground->transformToLocalSpace(position, scrollBar);
				}
			}
		}
	}

	void ScrollBarButtonSlider::_click(EventArgs* args)
	{
		if (args->baseObject != NULL)
		{
			ScrollBar* scrollBar = dynamic_cast<ScrollBar*>(args->baseObject->getParent());
			if (scrollBar != NULL)
			{
				scrollBar->_initAreaDragging();
				scrollBar->_adjustDragSpeed();
			}
		}
	}

}

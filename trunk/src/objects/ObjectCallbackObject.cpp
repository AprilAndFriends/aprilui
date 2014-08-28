/// @file
/// @version 3.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "ObjectCallbackObject.h"

namespace aprilui
{
	CallbackObject::CallbackObject(chstr name, grect rect) : Object(name, rect)
	{
		this->drawCallback = NULL;
		this->updateCallback = NULL;
	}

	CallbackObject::~CallbackObject()
	{
	}

	Object* CallbackObject::createInstance(chstr name, grect rect)
	{
		return new CallbackObject(name, rect);
	}

	void CallbackObject::OnDraw()
	{
		if (this->drawCallback != NULL)
		{
			(*this->drawCallback)(this);
		}
	}

	void CallbackObject::update(float timeDelta)
	{
		if (this->updateCallback != NULL)
		{
			(*this->updateCallback)(timeDelta);
		}
		Object::update(timeDelta);
	}
	
	bool CallbackObject::onMouseDown(april::Key keyCode)
	{
		if (Object::onMouseDown(keyCode))
		{
			return true;
		}
		if (this->isCursorInside())
		{
			this->triggerEvent(Event::MOUSE_DOWN, keyCode);
			return true;
		}
		return false;
	}

	bool CallbackObject::onMouseUp(april::Key keyCode)
	{
		if (Object::onMouseUp(keyCode))
		{
			return true;
		}
		if (this->isCursorInside())
		{
			this->triggerEvent(Event::CLICK, keyCode);
			return true;
		}
		return false;
	}
	
	bool CallbackObject::onMouseMove()
	{
		if (Object::onMouseMove())
		{
			return true;
		}
		if (this->isCursorInside())
		{
			this->triggerEvent(Event::MOUSE_MOVE);
		}
		return false;
	}
	
	bool CallbackObject::onMouseScroll(float x, float y)
	{
		if (Object::onMouseScroll(x, y))
		{
			return true;
		}
		if (this->isCursorInside())
		{
			this->triggerEvent(Event::MOUSE_SCROLL, april::AK_NONE, gvec2(x, y));
		}
		return false;
	}

	void CallbackObject::mouseCancel()
	{
		Object::mouseCancel();
		this->triggerEvent(Event::MOUSE_CANCEL);
	}

}

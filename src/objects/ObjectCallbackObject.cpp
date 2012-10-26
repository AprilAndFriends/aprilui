/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "ObjectCallbackObject.h"

namespace aprilui
{
	CallbackObject::CallbackObject(chstr name, grect rect) :
		Object(name, rect)
	{
		this->mDrawCallback = NULL;
		this->mUpdateCallback = NULL;
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
		if (this->mDrawCallback != NULL)
		{
			(*this->mDrawCallback)(this);
		}
	}

	void CallbackObject::update(float k)
	{
		if (this->mUpdateCallback != NULL)
		{
			(*this->mUpdateCallback)(k);
		}
		Object::update(k);
	}
	
	bool CallbackObject::onMouseDown(int button)
	{
		if (Object::onMouseDown(button))
		{
			return true;
		}
		if (this->isCursorInside())
		{
			this->triggerEvent("MouseDown", button);
			return true;
		}
		return false;
	}

	bool CallbackObject::onMouseUp(int button)
	{
		if (Object::onMouseUp(button))
		{
			return true;
		}
		if (this->isCursorInside())
		{
			this->triggerEvent("Click", button);
			return true;
		}
		return false;
	}
	
	bool CallbackObject::onMouseMove()
	{
		bool result = Object::onMouseMove();
		if (this->isCursorInside())
		{
			this->triggerEvent("MouseMove");
		}
		return result;
	}
	
	bool CallbackObject::onMouseScroll(float x, float y)
	{
		bool result = Object::onMouseScroll(x, y);
		if (this->isCursorInside())
		{
			this->triggerEvent("MouseScroll", x, y);
		}
		return result;
	}

	void CallbackObject::cancelMouseDown()
	{
		Object::cancelMouseDown();
		this->triggerEvent("CancelMouseDown");
	}

}

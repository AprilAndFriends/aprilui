/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.7
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
		mDrawCallback = NULL;
		mUpdateCallback = NULL;
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
		if (mDrawCallback != NULL)
		{
			(*mDrawCallback)(this);
		}
	}

	void CallbackObject::update(float k)
	{
		if (mUpdateCallback != NULL)
		{
			(*mUpdateCallback)(k);
		}
		Object::update(k);
	}
	
	bool CallbackObject::onMouseDown(int button)
	{
		if (Object::onMouseDown(button))
		{
			return true;
		}
		if (isCursorInside())
		{
			_triggerEvent("MouseDown", button);
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
		if (isCursorInside())
		{
			_triggerEvent("Click", button);
			return true;
		}
		return false;
	}
	
	void CallbackObject::onMouseMove()
	{
		Object::onMouseMove();
		if (isCursorInside())
		{
			_triggerEvent("MouseMove");
		}
	}
	
	void CallbackObject::onMouseScroll(float x, float y)
	{
		Object::onMouseScroll(x, y);
		if (isCursorInside())
		{
			_triggerEvent("MouseScroll", x, y);
		}
	}
}

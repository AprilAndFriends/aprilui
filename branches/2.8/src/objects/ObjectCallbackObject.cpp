/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.8
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
	
	bool CallbackObject::onMouseDown(april::Key keyCode)
	{
		if (Object::onMouseDown(keyCode))
		{
			return true;
		}
		if (this->isCursorInside())
		{
			this->triggerEvent("MouseDown", keyCode);
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
			this->triggerEvent("Click", keyCode);
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
			this->triggerEvent("MouseMove");
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
			this->triggerEvent("MouseScroll", x, y);
		}
		return false;
	}

	void CallbackObject::mouseCancel()
	{
		Object::mouseCancel();
		this->triggerEvent("MouseCancel");
	}

}

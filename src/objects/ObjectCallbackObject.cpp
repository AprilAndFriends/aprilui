/// @file
/// @version 5.0
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
	CallbackObject::CallbackObject(chstr name) : Object(name)
	{
		this->drawCallback = NULL;
		this->updateCallback = NULL;
		this->debugColor.a = 0;
	}

	CallbackObject::CallbackObject(const CallbackObject& other) : Object(other)
	{
		this->drawCallback = other.drawCallback;
		this->updateCallback = other.updateCallback;
		this->debugColor.a = 0;
	}

	CallbackObject::~CallbackObject()
	{
	}

	Object* CallbackObject::createInstance(chstr name)
	{
		return new CallbackObject(name);
	}

	void CallbackObject::_draw()
	{
		if (this->drawCallback != NULL)
		{
			(*this->drawCallback)(this);
		}
	}

	void CallbackObject::_update(float timeDelta)
	{
		if (this->updateCallback != NULL)
		{
			(*this->updateCallback)(timeDelta);
		}
		Object::_update(timeDelta);
	}
}

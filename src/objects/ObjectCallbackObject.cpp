/// @file
/// @version 3.4
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
}

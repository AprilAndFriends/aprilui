/// @file
/// @version 1.34
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
	CallbackObject::CallbackObject(chstr name, grect rect) :
		Object("CallbackObject", name, rect)
	{
		mCallback = NULL;
	}

	bool CallbackObject::setProperty(chstr name, chstr value)
	{
		return Object::setProperty(name, value);
	}
	
	void CallbackObject::OnDraw(gvec2 offset)
	{
		if (mCallback != NULL)
		{
			(*mCallback)();
		}
		Object::OnDraw(offset);
	}
	
}

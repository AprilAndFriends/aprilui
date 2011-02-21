/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "ObjectCallbackObject.h"

namespace aprilui
{
	CallbackObject::CallbackObject(chstr name, grect rect) :
		Object("CallbackObject", name, rect)
	{
		mCallback = NULL;
		mUpdateCallback = NULL;
	}

	void CallbackObject::setProperty(chstr name, chstr value)
	{
		Object::setProperty(name, value);
	}
	
	void CallbackObject::OnDraw()
	{
		if (mCallback != NULL)
		{
			(*mCallback)();
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
	
	bool CallbackObject::OnMouseDown(float x, float y, int button)
	{
		if (Object::OnMouseDown(x, y, button))
		{
			return true;
		}
		if (isCursorInside())
		{
			triggerEvent("MouseDown", x, y, 0);
			return true;
		}
		return false;
	}

	bool CallbackObject::OnMouseUp(float x, float y, int button)
	{
		if (Object::OnMouseUp(x, y, button))
		{
			return true;
		}
		if (isCursorInside())
		{
			triggerEvent("Click", x, y, 0);
			return true;
		}
		return false;
	}
}

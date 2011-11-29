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
		Object(name, rect)
	{
		mDrawCallback = NULL;
		mUpdateCallback = NULL;
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
	
	bool CallbackObject::onMouseDown(float x, float y, int button)
	{
		if (Object::onMouseDown(x, y, button))
		{
			return true;
		}
		if (isCursorInside())
		{
			_triggerEvent("MouseDown", x, y, button);
			return true;
		}
		return false;
	}

	bool CallbackObject::onMouseUp(float x, float y, int button)
	{
		if (Object::onMouseUp(x, y, button))
		{
			return true;
		}
		if (isCursorInside())
		{
			_triggerEvent("Click", x, y, button);
			return true;
		}
		return false;
	}
}

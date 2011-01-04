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
	}

	void CallbackObject::setProperty(chstr name, chstr value)
	{
		Object::setProperty(name, value);
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

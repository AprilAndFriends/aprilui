/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include "EventArgs.h"

namespace aprilui
{
	EventArgs::EventArgs(Object* object, float x, float y, april::Key keyCode, chstr extra)
	{
		this->object = object;
		this->x = x;
		this->y = y;
		this->keyCode = keyCode;
		this->buttonCode = april::AB_NONE;
		this->extra = extra;
	}

	EventArgs::EventArgs(Object* object, float x, float y, april::Button buttonCode, chstr extra)
	{
		this->object = object;
		this->x = x;
		this->y = y;
		this->keyCode = april::AK_NONE;
		this->buttonCode = buttonCode;
		this->extra = extra;
	}

	EventArgs::~EventArgs()
	{
	}
	
}

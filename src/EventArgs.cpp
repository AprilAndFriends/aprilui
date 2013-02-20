/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.5
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include "EventArgs.h"

namespace aprilui
{
	EventArgs::EventArgs(Object* object, float x, float y, april::Key keyCode, chstr extra)
	{
		this->object = object;
		this->x = x;
		this->y = y;
		this->keyCode = keyCode;
		this->extra = extra;
	}

	EventArgs::~EventArgs()
	{
	}
	
}

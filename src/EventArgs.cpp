/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include "EventArgs.h"
#include "Object.h"

namespace aprilui
{
	EventArgs::EventArgs(Object* o, float x_pos, float y_pos, unsigned int keycode, chstr extra_string)
	{
		this->object = o;
		this->x = x_pos;
		this->y = y_pos;
		this->keycode = keycode;
		this->extra = extra_string;
	}

	EventArgs::~EventArgs()
	{
	}
	
}

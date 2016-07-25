/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include "EventArgs.h"
#include "Object.h"

namespace aprilui
{
	EventArgs::EventArgs(Object* o, float x_pos, float y_pos, chstr extra_string)
	{
		object = o;
		x = x_pos;
		y = y_pos;
		extra = extra_string;
	}
	
}

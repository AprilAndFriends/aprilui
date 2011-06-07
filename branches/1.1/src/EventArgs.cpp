/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
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

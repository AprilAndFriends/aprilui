/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Defines an object for arguments of event callbacks.

#ifndef APRILUI_EVENT_ARGS_H
#define APRILUI_EVENT_ARGS_H

#include <stdio.h>

#include <hltypes/hstring.h>

#include "apriluiExport.h"

namespace aprilui
{
	class Object;

	class apriluiExport EventArgs
	{
	public:
		Object* object;
		float x;
		float y;
        unsigned int keycode;
		hstr extra;
		
		EventArgs(Object* o, float x_pos = 0.0f, float y_pos = 0.0f, unsigned int keycode = 0, chstr extra_string = "");
		~EventArgs();

	};

}

#endif
/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines an object for arguments of event callbacks.

#ifndef APRILUI_EVENT_ARGS_H
#define APRILUI_EVENT_ARGS_H

#include <hltypes/hstring.h>
#include <april/Keys.h>

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
		april::Key keyCode;
		april::Button buttonCode;
		hstr extra;
		
		EventArgs(Object* object, float x = 0.0f, float y = 0.0f, april::Key keyCode = april::AK_NONE, chstr extra = "");
		EventArgs(Object* object, float x, float y, april::Button buttonCode, chstr extra = "");
		~EventArgs();

	};

}

#endif

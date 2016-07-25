/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include "CallbackEvent.h"

namespace aprilui
{
	CallbackEvent::CallbackEvent(void (*callback)(EventArgs*))
	{
		mCallback = callback;
	}

	void CallbackEvent::execute(void* params)
	{
		mCallback((EventArgs*)params);
	}

}

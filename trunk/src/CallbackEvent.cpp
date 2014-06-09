/// @file
/// @version 3.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include "CallbackEvent.h"

namespace aprilui
{
	CallbackEvent::CallbackEvent(void (*callback)(EventArgs*)) : Event()
	{
		this->callback = callback;
	}

	CallbackEvent::~CallbackEvent()
	{
	}

	void CallbackEvent::execute(void* params)
	{
		(*this->callback)((EventArgs*)params);
	}

}

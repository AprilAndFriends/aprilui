/// @file
/// @version 5.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include "CallbackEvent.h"

namespace aprilui
{
	CallbackEvent::CallbackEvent(void (*callback)(EventArgs*)) :
		Event()
	{
		this->callback = callback;
	}

	CallbackEvent::CallbackEvent(const CallbackEvent& other) :
		Event(other)
	{
		this->callback = other.callback;
	}

	void CallbackEvent::execute(EventArgs* args)
	{
		(*this->callback)(args);
	}

}

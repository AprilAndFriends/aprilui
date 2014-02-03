/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

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

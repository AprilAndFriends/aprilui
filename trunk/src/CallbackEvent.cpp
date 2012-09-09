/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.0
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
		this->mCallback = callback;
	}

	CallbackEvent::~CallbackEvent()
	{
	}

	void CallbackEvent::execute(void* params)
	{
		(*this->mCallback)((EventArgs*)params);
	}

}

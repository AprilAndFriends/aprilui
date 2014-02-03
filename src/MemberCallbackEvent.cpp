/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include "MemberCallbackEvent.h"

namespace aprilui
{
	MemberCallbackEvent::~MemberCallbackEvent()
	{
		delete this->callback;
	}

	void MemberCallbackEvent::execute(void* params)
	{
		this->callback->execute((EventArgs*)params);
	}

}

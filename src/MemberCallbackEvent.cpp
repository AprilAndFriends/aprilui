/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.91
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
		delete this->mCallback;
	}

	void MemberCallbackEvent::execute(void* params)
	{
		this->mCallback->execute((EventArgs*)params);
	}

}

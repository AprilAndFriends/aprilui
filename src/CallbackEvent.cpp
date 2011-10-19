/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include "CallbackEvent.h"

namespace aprilui
{
	CallbackEvent::CallbackEvent(void (*callback)(EventArgs*)) : Event()
	{
		mCallback = callback;
	}

	CallbackEvent::~CallbackEvent()
	{
	}

	void CallbackEvent::execute(void* params)
	{
		mCallback((EventArgs*)params);
	}

}
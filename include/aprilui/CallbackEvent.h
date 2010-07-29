/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef CALLBACK_EVENT_H
#define CALLBACK_EVENT_H

#include "AprilUIExport.h"
#include "Event.h"

namespace AprilUI
{
	class EventArgs;
	
	class CallbackEvent : public Event
	{
		void (*mCallback)(EventArgs*);
	public:
		CallbackEvent(void (*callback)(EventArgs*));
		void execute(void* params);
	};

}

#endif

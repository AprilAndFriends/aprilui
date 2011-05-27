/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_CALLBACK_EVENT_H
#define APRILUI_CALLBACK_EVENT_H

#include "Event.h"

namespace aprilui
{
	class EventArgs;
	
	class CallbackEvent : public Event
	{
	public:
		CallbackEvent(void (*callback)(EventArgs*));
		void execute(void* params);
		
	protected:
		void (*mCallback)(EventArgs*);
		
	};

}

#endif

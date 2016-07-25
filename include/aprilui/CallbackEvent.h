/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

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

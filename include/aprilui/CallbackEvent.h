/// @file
/// @version 5.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines a callback event.

#ifndef APRILUI_CALLBACK_EVENT_H
#define APRILUI_CALLBACK_EVENT_H

#include "Event.h"

namespace aprilui
{
	class EventArgs;
	
	class apriluiExport CallbackEvent : public Event
	{
		APRILUI_CLONEABLE(CallbackEvent);
	public:
		CallbackEvent(void (*callback)(EventArgs*));

		void execute(EventArgs* args);
		
	protected:
		void (*callback)(EventArgs*);
		
	};

}
#endif

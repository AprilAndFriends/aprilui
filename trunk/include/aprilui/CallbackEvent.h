/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.5
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
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
	public:
		CallbackEvent(void (*callback)(EventArgs*));
		~CallbackEvent();

		void execute(void* params);
		
	protected:
		void (*mCallback)(EventArgs*);
		
	};

}

#endif

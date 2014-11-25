/// @file
/// @version 3.6
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines a member callback event.

#ifndef APRILUI_MEMBER_CALLBACK_EVENT_H
#define APRILUI_MEMBER_CALLBACK_EVENT_H

#include "Event.h"
#include "apriluiExport.h"

namespace aprilui
{
	class EventArgs;
	
	template <typename T>
	class MemberCallbackEvent : public Event
	{
	public:
		inline MemberCallbackEvent(void (T::*function)(EventArgs*), T* object) : Event()
		{
			this->object = object;
			this->function = function;
		}

		inline ~MemberCallbackEvent()
		{
		}

		inline void execute(EventArgs* args)
		{
			(this->object->*function)(args);
		}

	protected:
		T* object;
		void (T::*function)(EventArgs*);

	};

}

#endif

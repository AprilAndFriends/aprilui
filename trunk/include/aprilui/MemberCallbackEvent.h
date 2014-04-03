/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 3.0
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
	
	class apriluiExport MemberCallbackEventBase
	{
	public:
		MemberCallbackEventBase() { }
		virtual ~MemberCallbackEventBase() { }

		virtual void execute(void* params) = 0;

	};

	template <typename T>
	class MemberCallbackEventImpl : public MemberCallbackEventBase
	{
	public:
		MemberCallbackEventImpl(void (T::*function)(EventArgs*), T* object) : MemberCallbackEventBase()
		{
			this->function = function;
			this->object = object;
		}
		
		~MemberCallbackEventImpl()
		{
		}

		void execute(void* params)
		{
			(this->object->*function)((EventArgs*)params);
		}

	protected:
		void (T::*function)(EventArgs*);
		T* object;

	};

	class apriluiExport MemberCallbackEvent : public Event
	{
	public:
		template <typename T>
		MemberCallbackEvent(void (T::*function)(EventArgs*), T* obj) : Event()
		{
			this->callback = new MemberCallbackEventImpl<T>(function, obj);
		}
		~MemberCallbackEvent();

		void execute(void* params);

	protected:
		MemberCallbackEventBase* callback;

	};

}

#endif

/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_MEMBER_CALLBACK_EVENT_H
#define APRILUI_MEMBER_CALLBACK_EVENT_H

#include "Event.h"
#include "AprilUIExport.h"

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
			mFunction = function;
			mObject = object;
		}
		
		~MemberCallbackEventImpl()
		{
		}

		void execute(void* params)
		{
			(mObject->*mFunction)((EventArgs*)params);
		}

	protected:
		void (T::*mFunction)(EventArgs*);
		T* mObject;

	};

	class apriluiExport MemberCallbackEvent : public Event
	{
	public:
		template <typename T>
	    MemberCallbackEvent(void (T::*function)(EventArgs*), T* obj) : Event()
		{
			mCallback = new MemberCallbackEventImpl<T>(function, obj);
		}
		~MemberCallbackEvent();

		void execute(void* params);

	protected:
		MemberCallbackEventBase* mCallback;

	};

}

#endif

/// @file
/// @version 5.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <gtypes/Vector2.h>
#include <hltypes/harray.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "CallbackEvent.h"
#include "Dataset.h"
#include "Event.h"
#include "EventArgs.h"
#include "EventReceiver.h"

namespace aprilui
{
	EventReceiver::EventReceiver() :
		Cloneable()
	{
		this->dataset = NULL;
	}

	EventReceiver::EventReceiver(const EventReceiver& other) :
		Cloneable(other)
	{
		this->dataset = NULL;
		foreachc_m (Event*, it, other.events)
		{
			this->events[it->first] = it->second->clone();
		}
	}
	
	EventReceiver::~EventReceiver()
	{
		foreach_m (Event*, it, this->events)
		{
			if (this->dataset != NULL)
			{
				this->dataset->removeCallbackFromQueue(it->second);
			}
			delete it->second;
		}
		this->events.clear();
	}

	bool EventReceiver::registerEvent(chstr type, void(*callback)(EventArgs*))
	{
		CallbackEvent* event = new CallbackEvent(callback);
		bool result = this->registerEvent(type, event);
		if (!result && event != NULL)
		{
			delete event;
		}
		return result;
	}

	bool EventReceiver::registerEvent(chstr type, Event* event)
	{
		if (event != NULL)
		{
			this->unregisterEvent(type);
			this->events[type] = event;
			return true;
		}
		return false;
	}

	bool EventReceiver::unregisterEvent(chstr type)
	{
		if (this->events.hasKey(type))
		{
			Event* event = this->events[type];
			if (this->dataset != NULL)
			{
				this->dataset->removeCallbackFromQueue(event);
			}
			delete event;
			this->events.removeKey(type);
			return true;
		}
		return false;
	}

	bool EventReceiver::isEventRegistered(chstr type) const
	{
		return this->events.hasKey(type);
	}

	void EventReceiver::notifyEvent(chstr type, EventArgs* args)
	{
	}

	bool EventReceiver::triggerEvent(chstr type, april::Key keyCode)
	{
		if (this->events.hasKey(type))
		{
			this->dataset->queueCallback(this->events[type], new EventArgs(type, this, keyCode, aprilui::getCursorPosition(), "", NULL));
			return true;
		}
		return false;
	}

	bool EventReceiver::triggerEvent(chstr type, april::Key keyCode, chstr string)
	{
		if (this->events.hasKey(type))
		{
			this->dataset->queueCallback(this->events[type], new EventArgs(type, this, keyCode, aprilui::getCursorPosition(), string, NULL));
			return true;
		}
		return false;
	}

	bool EventReceiver::triggerEvent(chstr type, april::Key keyCode, cgvec2f position, chstr string, void* userData)
	{
		if (this->events.hasKey(type))
		{
			this->dataset->queueCallback(this->events[type], new EventArgs(type, this, keyCode, position, string, userData));
			return true;
		}
		return false;
	}

	bool EventReceiver::triggerEvent(chstr type, april::Button buttonCode, chstr string, void* userData)
	{
		if (this->events.hasKey(type))
		{
			this->dataset->queueCallback(this->events[type], new EventArgs(type, this, buttonCode, string, userData));
			return true;
		}
		return false;
	}

	bool EventReceiver::triggerEvent(chstr type, chstr string, void* userData)
	{
		if (this->events.hasKey(type))
		{
			this->dataset->queueCallback(this->events[type], new EventArgs(type, this, string, userData));
			return true;
		}
		return false;
	}

	bool EventReceiver::triggerEvent(chstr type, void* userData)
	{
		if (this->events.hasKey(type))
		{
			this->dataset->queueCallback(this->events[type], new EventArgs(type, this, userData));
			return true;
		}
		return false;
	}

}

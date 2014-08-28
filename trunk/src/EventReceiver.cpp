/// @file
/// @version 3.3
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
	EventReceiver::EventReceiver()
	{
		this->dataset = NULL;
	}
	
	EventReceiver::~EventReceiver()
	{
	}
	
	bool EventReceiver::registerEvent(Event::Type type, void(*callback)(EventArgs*))
	{
		CallbackEvent* event = new CallbackEvent(callback);
		bool result = this->registerEvent(type, event);
		if (!result)
		{
			delete event;
		}
		return result;
	}

	bool EventReceiver::registerEvent(Event::Type type, Event* event)
	{
		if (event != NULL)
		{
			this->unregisterEvent(type);
			this->events[type] = event;
			return true;
		}
		return false;
	}

	bool EventReceiver::unregisterEvent(Event::Type type)
	{
		if (this->events.has_key(type))
		{
			Event* event = this->events[type];
			if (this->dataset != NULL)
			{
				this->dataset->removeCallbackFromQueue(event);
			}
			delete event;
			this->events.remove_key(type);
			return true;
		}
		return false;
	}

	bool EventReceiver::registerEvent(chstr customType, void(*callback)(EventArgs*))
	{
		CallbackEvent* event = new CallbackEvent(callback);
		bool result = this->registerEvent(customType, event);
		if (!result)
		{
			delete event;
		}
		return result;
	}

	bool EventReceiver::registerEvent(chstr customType, Event* event)
	{
		if (event != NULL)
		{
			this->unregisterEvent(customType);
			this->customEvents[customType] = event;
			return true;
		}
		return false;
	}

	bool EventReceiver::unregisterEvent(chstr customType)
	{
		if (this->customEvents.has_key(customType))
		{
			Event* event = this->customEvents[customType];
			if (this->dataset != NULL)
			{
				this->dataset->removeCallbackFromQueue(event);
			}
			delete event;
			this->customEvents.remove_key(customType);
			return true;
		}
		return false;
	}

	void EventReceiver::notifyEvent(Event::Type type, EventArgs* args)
	{
	}

	void EventReceiver::notifyEvent(chstr customType, EventArgs* args)
	{
	}

	bool EventReceiver::triggerEvent(Event::Type type, april::Key keyCode)
	{
		if (this->events.has_key(type))
		{
			this->dataset->queueCallback(this->events[type], new EventArgs(this, keyCode, aprilui::getCursorPosition(), "", NULL));
			return true;
		}
		return false;
	}

	bool EventReceiver::triggerEvent(Event::Type type, april::Key keyCode, chstr string)
	{
		if (this->events.has_key(type))
		{
			this->dataset->queueCallback(this->events[type], new EventArgs(this, keyCode, aprilui::getCursorPosition(), string, NULL));
			return true;
		}
		return false;
	}

	bool EventReceiver::triggerEvent(Event::Type type, april::Key keyCode, gvec2 position, chstr string, void* userData)
	{
		if (this->events.has_key(type))
		{
			this->dataset->queueCallback(this->events[type], new EventArgs(this, keyCode, position, string, userData));
			return true;
		}
		return false;
	}

	bool EventReceiver::triggerEvent(Event::Type type, april::Button buttonCode, chstr string, void* userData)
	{
		if (this->events.has_key(type))
		{
			this->dataset->queueCallback(this->events[type], new EventArgs(this, buttonCode, string, userData));
			return true;
		}
		return false;
	}

	bool EventReceiver::triggerEvent(Event::Type type, chstr string, void* userData)
	{
		if (this->events.has_key(type))
		{
			this->dataset->queueCallback(this->events[type], new EventArgs(this, string, userData));
			return true;
		}
		return false;
	}

	bool EventReceiver::triggerEvent(Event::Type type, void* userData)
	{
		if (this->events.has_key(type))
		{
			this->dataset->queueCallback(this->events[type], new EventArgs(this, userData));
			return true;
		}
		return false;
	}

	bool EventReceiver::triggerEvent(chstr customType, april::Key keyCode)
	{
		if (this->customEvents.has_key(customType))
		{
			this->dataset->queueCallback(this->customEvents[customType], new EventArgs(this, keyCode, aprilui::getCursorPosition(), "", NULL));
			return true;
		}
		return false;
	}

	bool EventReceiver::triggerEvent(chstr customType, april::Key keyCode, chstr string)
	{
		if (this->customEvents.has_key(customType))
		{
			this->dataset->queueCallback(this->customEvents[customType], new EventArgs(this, keyCode, aprilui::getCursorPosition(), string, NULL));
			return true;
		}
		return false;
	}

	bool EventReceiver::triggerEvent(chstr customType, april::Key keyCode, gvec2 position, chstr string, void* userData)
	{
		if (this->customEvents.has_key(customType))
		{
			this->dataset->queueCallback(this->customEvents[customType], new EventArgs(this, keyCode, position, string, userData));
			return true;
		}
		return false;
	}

	bool EventReceiver::triggerEvent(chstr customType, april::Button buttonCode, chstr string, void* userData)
	{
		if (this->customEvents.has_key(customType))
		{
			this->dataset->queueCallback(this->customEvents[customType], new EventArgs(this, buttonCode, string, userData));
			return true;
		}
		return false;
	}

	bool EventReceiver::triggerEvent(chstr customType, chstr string, void* userData)
	{
		if (this->customEvents.has_key(customType))
		{
			this->dataset->queueCallback(this->customEvents[customType], new EventArgs(this, string, userData));
			return true;
		}
		return false;
	}

	bool EventReceiver::triggerEvent(chstr customType, void* userData)
	{
		if (this->customEvents.has_key(customType))
		{
			this->dataset->queueCallback(this->customEvents[customType], new EventArgs(this, userData));
			return true;
		}
		return false;
	}

}

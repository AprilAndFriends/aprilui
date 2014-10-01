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

#define EVENT_TYPE_CHECK_DEPRECATED(newType, oldType) \
	if (realType == oldType) \
	{ \
		hlog::warnf(aprilui::logTag, "The event '%s' is deprecated. Used '%s' instead.", oldType, newType.c_str()); \
		realType = newType; \
	}

namespace aprilui
{
	EventReceiver::EventReceiver()
	{
		this->dataset = NULL;
	}
	
	EventReceiver::~EventReceiver()
	{
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
			/*
			this->unregisterEvent(type);
			this->events[type] = event;
			*/
			// TODO - will be removed once deprecated code is removed
			hstr realType = type;
			EVENT_TYPE_CHECK_DEPRECATED(Event::RegisteredInDataset, "RegisterInDataset");
			EVENT_TYPE_CHECK_DEPRECATED(Event::UnregisteredFromDataset, "UnregisterFromDataset");
			EVENT_TYPE_CHECK_DEPRECATED(Event::AttachedToObject, "AttachToObject");
			EVENT_TYPE_CHECK_DEPRECATED(Event::DetachedFromObject, "DetachFromObject");
			EVENT_TYPE_CHECK_DEPRECATED(Event::EnabledChanged, "OnEnableChanged");
			EVENT_TYPE_CHECK_DEPRECATED(Event::FocusGained, "GainFocus");
			EVENT_TYPE_CHECK_DEPRECATED(Event::FocusLost, "LoseFocus");
			EVENT_TYPE_CHECK_DEPRECATED(Event::TextChanged, "onTextChanged");
			EVENT_TYPE_CHECK_DEPRECATED(Event::TextKeyChanged, "onTextKeyChanged");
			EVENT_TYPE_CHECK_DEPRECATED(Event::LocalizationChanged, "onLocalizationChanged");
			EVENT_TYPE_CHECK_DEPRECATED(Event::SubmitEditText, "Submit");
			EVENT_TYPE_CHECK_DEPRECATED(Event::ScrollSkinChanged, "SkinChange");
			EVENT_TYPE_CHECK_DEPRECATED(Event::SetSliderValue, "Set");
			EVENT_TYPE_CHECK_DEPRECATED(Event::DelayExpired, "OnDelayEnd");
			this->unregisterEvent(realType);
			this->events[realType] = event;
			// TODO - until here
			return true;
		}
		return false;
	}

	bool EventReceiver::unregisterEvent(chstr type)
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

	bool EventReceiver::isEventRegistered(chstr type)
	{
		return this->events.has_key(type);
	}

	void EventReceiver::notifyEvent(chstr type, EventArgs* args)
	{
	}

	bool EventReceiver::triggerEvent(chstr type, april::Key keyCode)
	{
		if (this->events.has_key(type))
		{
			this->dataset->queueCallback(this->events[type], new EventArgs(this, keyCode, aprilui::getCursorPosition(), "", NULL));
			return true;
		}
		return false;
	}

	bool EventReceiver::triggerEvent(chstr type, april::Key keyCode, chstr string)
	{
		if (this->events.has_key(type))
		{
			this->dataset->queueCallback(this->events[type], new EventArgs(this, keyCode, aprilui::getCursorPosition(), string, NULL));
			return true;
		}
		return false;
	}

	bool EventReceiver::triggerEvent(chstr type, april::Key keyCode, gvec2 position, chstr string, void* userData)
	{
		if (this->events.has_key(type))
		{
			this->dataset->queueCallback(this->events[type], new EventArgs(this, keyCode, position, string, userData));
			return true;
		}
		return false;
	}

	bool EventReceiver::triggerEvent(chstr type, april::Button buttonCode, chstr string, void* userData)
	{
		if (this->events.has_key(type))
		{
			this->dataset->queueCallback(this->events[type], new EventArgs(this, buttonCode, string, userData));
			return true;
		}
		return false;
	}

	bool EventReceiver::triggerEvent(chstr type, chstr string, void* userData)
	{
		if (this->events.has_key(type))
		{
			this->dataset->queueCallback(this->events[type], new EventArgs(this, string, userData));
			return true;
		}
		return false;
	}

	bool EventReceiver::triggerEvent(chstr type, void* userData)
	{
		if (this->events.has_key(type))
		{
			this->dataset->queueCallback(this->events[type], new EventArgs(this, userData));
			return true;
		}
		return false;
	}

}

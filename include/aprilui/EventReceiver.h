/// @file
/// @version 3.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines a base class for classes that can receive and handle events.

#ifndef APRILUI_EVENT_RECEIVER_H
#define APRILUI_EVENT_RECEIVER_H

#include <april/Keys.h>
#include <gtypes/Vector2.h>
#include <hltypes/harray.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "Event.h"

namespace aprilui
{
	class Dataset;
	class EventArgs;

	class apriluiExport EventReceiver
	{
	public:
		EventReceiver();
		virtual ~EventReceiver();

		inline hmap<Event::Type, Event*>& getEvents() { return this->events; }
		inline hmap<hstr, Event*>& getCustomEvents() { return this->customEvents; }

		bool registerEvent(Event::Type type, void(*callback)(EventArgs*));
		bool registerEvent(Event::Type type, Event* event);
		bool unregisterEvent(Event::Type type);
		bool registerEvent(chstr customType, void(*callback)(EventArgs*));
		bool registerEvent(chstr customType, Event* event);
		bool unregisterEvent(chstr customType);

		virtual void notifyEvent(Event::Type type, EventArgs* args);
		virtual void notifyEvent(chstr customType, EventArgs* args);

		virtual bool triggerEvent(Event::Type type, april::Key keyCode);
		virtual bool triggerEvent(Event::Type type, april::Key keyCode, chstr string);
		virtual bool triggerEvent(Event::Type type, april::Key keyCode, gvec2 position, chstr string = "", void* userData = NULL);
		virtual bool triggerEvent(Event::Type type, april::Button buttonCode, chstr string, void* userData = NULL);
		virtual bool triggerEvent(Event::Type type, chstr string, void* userData = NULL);
		virtual bool triggerEvent(Event::Type type, void* userData = NULL);
		virtual bool triggerEvent(chstr customType, april::Key keyCode);
		virtual bool triggerEvent(chstr customType, april::Key keyCode, chstr string);
		virtual bool triggerEvent(chstr customType, april::Key keyCode, gvec2 position, chstr string = "", void* userData = NULL);
		virtual bool triggerEvent(chstr customType, april::Button buttonCode, chstr string, void* userData = NULL);
		virtual bool triggerEvent(chstr customType, chstr string, void* userData = NULL);
		virtual bool triggerEvent(chstr customType, void* userData = NULL);

	protected:
		Dataset* dataset;
		hmap<Event::Type, Event*> events;
		hmap<hstr, Event*> customEvents;

	};

}
#endif

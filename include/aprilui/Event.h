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
/// Defines an event.

#ifndef APRILUI_EVENT_H
#define APRILUI_EVENT_H

#include <hltypes/hstring.h>

#include "apriluiExport.h"

namespace aprilui
{
	class EventArgs;

	class apriluiExport Event
	{
	public:
		enum Type
		{
			UNDEFINED = 0,
			REGISTERED_IN_DATASET,
			UNREGISTERED_FROM_DATASET,
			ATTACHED_TO_OBJECT,
			DETACHED_FROM_OBJECT,
			MOUSE_DOWN,
			MOUSE_UP,
			MOUSE_MOVE,
			MOUSE_SCROLL,
			MOUSE_CANCEL,
			CLICK,
			KEY_DOWN,
			KEY_UP,
			BUTTON_DOWN,
			BUTTON_UP,
			BUTTON_TRIGGER,
			ENABLED_CHANGED,
			RESIZED,
			FOCUS_GAINED,
			FOCUS_LOST,
			TEXT_CHANGED,
			TEXT_KEY_CHANGED,
			LOCALIZATION_CHANGED,
			SUBMIT_EDIT_TEXT,
			UPDATE_IMAGE,
			SCROLL_SKIN_CHANGED,
			SET_SLIDER_VALUE,
			DELAY_EXPIRED,
		};

		Event();
		virtual ~Event();

		virtual void execute(EventArgs* args) = 0;

		static hstr typeToName(Type type, bool ignoreWarning = false);
		static Type nameToType(chstr name, bool ignoreWarning = false);
		
	};

}

#endif

/// @file
/// @author  Boris Mikic
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Defines a base class for classes that can receive and handle events.

#ifndef APRILUI_EVENT_RECEIVER_H
#define APRILUI_EVENT_RECEIVER_H

#include <hltypes/hstring.h>

#include "apriluiExport.h"

namespace aprilui
{
	// TODO - implement enumerated integrated events and custom events handling
	class apriluiExport EventReceiver
	{
	public:
		EventReceiver();
		virtual ~EventReceiver();

		virtual void notifyEvent(chstr name, void* params);
		
	};

}
#endif

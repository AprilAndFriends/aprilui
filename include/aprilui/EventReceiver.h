/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

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

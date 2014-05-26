/// @file
/// @version 3.0
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

#include "apriluiExport.h"

namespace aprilui
{
	class apriluiExport Event
	{
	public:
		inline Event() { }
		inline virtual ~Event() { }

		virtual void execute(void* params) = 0;
		
	};

}

#endif

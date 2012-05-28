/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_EVENT_H
#define APRILUI_EVENT_H

#include "apriluiExport.h"

namespace aprilui
{
	class apriluiExport Event
	{
	public:
		virtual void execute(void* params) = 0;
		virtual ~Event() {};
	};

}

#endif

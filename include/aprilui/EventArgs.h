/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_EVENT_ARGS_H
#define APRILUI_EVENT_ARGS_H

#include "AprilUIExport.h"

namespace AprilUI
{
	class Object;

	class AprilUIExport EventArgs
	{
	public:
		EventArgs(Object* o,float x_pos=0,float y_pos=0,char* extra_string=0) { x=x_pos; y=y_pos; object=o; extra=extra_string; }
		Object* object;
		float x,y;
		char* extra;
	};

}

#endif

/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_DUMMY_OBJECT_H
#define APRILUI_DUMMY_OBJECT_H

#include <hltypes/hstring.h>

#include "AprilUIExport.h"
#include "Object.h"

namespace AprilUI
{
	class AprilUIExport DummyObject : public Object
	{
	public:
		DummyObject(chstr name,float x,float y,float w,float h);
		void setProperty(chstr name,chstr value);
	};
	
}

#endif

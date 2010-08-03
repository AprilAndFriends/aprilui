/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_CALLBACK_OBJECT_H
#define APRILUI_CALLBACK_OBJECT_H

#include <hltypes/hstring.h>

#include "AprilUIExport.h"
#include "ObjectObject.h"

namespace AprilUI
{
	class AprilUIExport CallbackObject : public Object
	{
		void (*mCallback)();
	public:
		CallbackObject(chstr name,float x,float y,float w,float h);
		void setProperty(chstr name,chstr value);
		
		void setCallback(void (*callback)()) { mCallback=callback; }
		void (*getCallback())() { return mCallback; }
		
		void OnDraw(float offset_x=0,float offset_y=0);
	};
	
}

#endif

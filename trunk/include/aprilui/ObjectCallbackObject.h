/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_CALLBACK_OBJECT_H
#define APRILUI_CALLBACK_OBJECT_H

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "Object.h"

namespace aprilui
{
	class apriluiExport CallbackObject : public Object
	{
	public:
		CallbackObject(chstr name, grect rect);
		bool setProperty(chstr name, chstr value);
		
		void (*getDrawCallback())(CallbackObject*) { return mDrawCallback; }
		void setDrawCallback(void (*value)(CallbackObject*)) { mDrawCallback = value; }
		void setUpdateCallback(void (*value)(float)) { mUpdateCallback = value; }
		
		void OnDraw();
		void update(float k);
		bool onMouseDown(float x, float y, int button);
		bool onMouseUp(float x, float y, int button);
		
	protected:
		void (*mDrawCallback)(CallbackObject*);
		void (*mUpdateCallback)(float);

	};
}

#endif

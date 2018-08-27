/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

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
		CallbackObject(chstr name, grectf rect);
		bool setProperty(chstr name, chstr value);
		
		void (*getCallback())() { return mCallback; }
		void setCallback(void (*value)()) { mCallback = value; }
		
		void OnDraw(gvec2f offset = gvec2f());
		
	protected:
		void (*mCallback)();
		
	};
}

#endif

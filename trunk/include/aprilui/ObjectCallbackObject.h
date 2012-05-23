/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.7
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Defines a callback object.

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
		~CallbackObject();
		static Object* createInstance(chstr name, grect rect);

		void (*getDrawCallback())(CallbackObject*) { return mDrawCallback; }
		void setDrawCallback(void (*value)(CallbackObject*)) { mDrawCallback = value; }
		void setUpdateCallback(void (*value)(float)) { mUpdateCallback = value; }
		
		void OnDraw();
		void update(float k);
		bool onMouseDown(int button);
		bool onMouseUp(int button);
		void onMouseMove();
		void onMouseScroll(float x, float y);

	protected:
		void (*mDrawCallback)(CallbackObject*);
		void (*mUpdateCallback)(float);

	};
}

#endif

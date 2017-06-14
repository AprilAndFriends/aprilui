/// @file
/// @version 4.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines a rectangle object.

#ifndef APRILUI_RECT_H
#define APRILUI_RECT_H

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "Object.h"

namespace aprilui
{
	class apriluiExport Rect : public Object
	{
		APRILUI_CLONEABLE(Rect);
	public:
		Rect(chstr name);
		~Rect();
		inline hstr getClassName() const { return "Rect"; }

		static Object* createInstance(chstr name);

	protected:
		void _draw();

	};
}

#endif

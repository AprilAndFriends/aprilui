/// @file
/// @version 4.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines a filled rectangle object.

#ifndef APRILUI_FILLED_RECT_H
#define APRILUI_FILLED_RECT_H

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "Object.h"

namespace aprilui
{
	class apriluiExport FilledRect : public Object
	{
		APRILUI_CLONEABLE(FilledRect);
	public:
		FilledRect(chstr name);
		~FilledRect();
		inline hstr getClassName() const { return "FilledRect"; }

		static Object* createInstance(chstr name);

	protected:
		void _draw();

	};
}

#endif

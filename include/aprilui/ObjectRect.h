/// @file
/// @version 5.1
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
#include "ObjectColored.h"

#define APRILUI_COLOR_MAX_VERTICES_RECT 5

namespace aprilui
{
	class apriluiExport Rect : public Colored
	{
		APRILUI_CLONEABLE(Rect);
	public:
		Rect(chstr name);
		inline hstr getClassName() const { return "Rect"; }
		static Object* createInstance(chstr name);

	protected:
		april::ColoredVertex vertices[APRILUI_COLOR_MAX_VERTICES_RECT];

		void _draw();

	};

}
#endif

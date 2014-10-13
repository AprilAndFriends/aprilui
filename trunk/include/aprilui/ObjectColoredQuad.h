/// @file
/// @version 3.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines a color-quad object.

#ifndef APRILUI_COLORED_QUAD_H
#define APRILUI_COLORED_QUAD_H

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "Object.h"

namespace aprilui
{
	class apriluiExport ColoredQuad : public Object
	{
	public:
		ColoredQuad(chstr name, grect rect);
		~ColoredQuad();
		inline hstr getClassName() const { return "ColoredQuad"; }

		static Object* createInstance(chstr name, grect rect);

	protected:
		void _draw();

	};
}

#endif

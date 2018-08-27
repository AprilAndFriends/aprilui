/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifndef APRILUI_COLORED_QUAD_H
#define APRILUI_COLORED_QUAD_H

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "Object.h"

namespace aprilui
{
	class ColoredQuad : public Object
	{
	public:
		ColoredQuad(chstr name, grectf rect);
		
		void setColor(float a, float r, float g, float b);
		bool setProperty(chstr name, chstr value);
		
		void OnDraw(gvec2f offset = gvec2f());

		bool OnMouseDown(float x, float y, int button);
		bool OnMouseUp(float x, float y, int button);
		
	protected:
		april::Color mColor;
		
	};
}

#endif

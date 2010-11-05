/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_COLORED_QUAD_H
#define APRILUI_COLORED_QUAD_H

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "AprilUIExport.h"
#include "Object.h"

namespace AprilUI
{
	class ColoredQuad : public Object
	{
	public:
		ColoredQuad(chstr name,grect rect);
		
		void setColor(float a, float r, float g, float b);
		void setProperty(chstr name, chstr value);
		
		void OnDraw(float offset_x, float offset_y);

		bool OnMouseDown(float x, float y, int button);
		bool OnMouseUp(float x, float y, int button);
		
	protected:
		April::Color mColor;
		
	};
}

#endif

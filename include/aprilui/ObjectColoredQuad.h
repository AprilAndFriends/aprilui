/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_COLORED_QUAD_H
#define APRILUI_COLORED_QUAD_H

#include <map>

#include <april/RenderSystem.h>
#include <hltypes/hstring.h>

#include "AprilUIExport.h"
#include "ObjectObject.h"

namespace AprilUI
{
	class ColoredQuad : public Object
	{
		April::Color mColor;
	public:
		ColoredQuad(chstr name,float x,float y,float w,float h);
		void setColor(float a,float r,float g,float b);
		
		void OnDraw(float offset_x,float offset_y);
		void setProperty(chstr name,chstr value);

		bool OnMouseDown(int button,float x,float y);
		bool OnMouseUp(int button,float x,float y);
	};
	
}

#endif

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

#include "apriluiExport.h"
#include "ObjectUi.h"

namespace aprilui
{
	class ColoredQuad : public ObjectUi
	{
	public:
		ColoredQuad(chstr name, grect rect);
		
		void setProperty(chstr name, chstr value);
		
		void OnDraw(gvec2 offset = gvec2());

		bool OnMouseDown(float x, float y, int button);
		bool OnMouseUp(float x, float y, int button);
		
	};
}

#endif

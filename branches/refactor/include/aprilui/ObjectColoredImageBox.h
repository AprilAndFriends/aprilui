/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_COLORED_IMAGE_BOX_H
#define APRILUI_COLORED_IMAGE_BOX_H

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "AprilUIExport.h"
#include "ObjectImageBox.h"

namespace AprilUI
{
	class AprilUIExport ColoredImageBox : public ImageBox
	{
	protected:
		April::Color mColor;
		
		void OnDraw(float offset_x,float offset_y);
	public:
		ColoredImageBox(chstr name,grect rect);
		
		void setColor(chstr color);
		void setColor(April::Color color) { mColor=color; } ;
		April::Color getColor() { return mColor; };

		void setProperty(chstr name,chstr value);
	};
	
}

#endif

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

#include "apriluiExport.h"
#include "ObjectImageBox.h"

namespace aprilui
{
	class apriluiExport ColoredImageBox : public ImageBox
	{
	public:
		ColoredImageBox(chstr name, grect rect);
		
		April::Color getColor() { return mColor; }
		void setColor(April::Color value) { mColor = value; }
		void setColor(chstr value) { mColor.setColor(value); }

		void setProperty(chstr name, chstr value);
		
	protected:
		April::Color mColor;
		
		void OnDraw(gvec2 offset = gvec2());
		
	};
}

#endif

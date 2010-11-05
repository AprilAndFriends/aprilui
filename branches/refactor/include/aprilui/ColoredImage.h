/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_COLORED_IMAGE_H
#define APRILUI_COLORED_IMAGE_H

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>

#include "Image.h"

#include "AprilUIExport.h"

namespace AprilUI
{
	class AprilUIExport ColoredImage : public Image
	{
	public:
		ColoredImage(April::Texture* texture, chstr name, grect source, bool vertical = false, unsigned int color = 0xFFFFFFFF);
		
		void draw(grect rect, float r, float g, float b, float a);
		void draw(grect rect, float r, float g, float b, float a, float angle);

		float getAlpha();
		void setAlpha(float alpha);
		void setColor(April::Color value) { mColor = value; }
		void setColor(float a, float r, float g, float b);
		
	protected:
		April::Color mColor;
		
	};

}
#endif

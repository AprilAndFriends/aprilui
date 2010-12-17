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
#include <gtypes/Vector2.h>

#include "Image.h"

#include "apriluiExport.h"

namespace aprilui
{
	class apriluiExport ColoredImage : public Image
	{
	public:
		ColoredImage(april::Texture* texture, chstr name, grect source, bool vertical = false, april::Color color = april::Color::WHITE);
		
		void draw(grect rect, april::Color color);
		void draw(grect rect, april::Color color, float angle, gvec2 center);

		float getAlpha();
		void setAlpha(float alpha);
		void setColor(april::Color value) { mColor = value; }
		
	protected:
		april::Color mColor;
		
	};

}
#endif

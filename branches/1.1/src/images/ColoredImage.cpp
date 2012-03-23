/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>

#include "ColoredImage.h"

namespace aprilui
{
	ColoredImage::ColoredImage(april::Texture* texture, chstr name, grect source, bool vertical, april::Color color) :
				  Image(texture, name, source, vertical)
	{
		mColor = color;
	}

	float ColoredImage::getAlpha()
	{
		return mColor.a_f();
	}
	
	void ColoredImage::setAlpha(float alpha)
	{
		mColor.a = alpha * 255.0f;
	}
	
	void ColoredImage::draw(grect rect, april::Color color)
	{
		Image::draw(rect, mColor * color);
	}

	void ColoredImage::draw(grect rect, april::Color color, float angle, gvec2 center)
	{
		Image::draw(rect, mColor * color, angle, center);
	}

	void ColoredImage::setColor(float a, float r, float g, float b)
	{
		mColor.set(COLOR_COMP_FOR_NEW_APRIL(r), 
				   COLOR_COMP_FOR_NEW_APRIL(g), 
				   COLOR_COMP_FOR_NEW_APRIL(b), 
				   COLOR_COMP_FOR_NEW_APRIL(a));
	}

}
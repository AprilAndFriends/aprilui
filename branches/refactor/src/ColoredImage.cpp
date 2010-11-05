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

#include "ColoredImage.h"

namespace AprilUI
{
	ColoredImage::ColoredImage(April::Texture* texture, chstr name, grect source, bool vertical, April::Color color) :
				  Image(texture, name, source, vertical)
	{
		mColor = color;
	}

	float ColoredImage::getAlpha()
	{
		return mColor.a_float();
	}
	
	void ColoredImage::setAlpha(float alpha)
	{
		mColor.a = alpha * 255.0f;
	}
		
	void ColoredImage::draw(grect rect, April::Color color)
	{
		Image::draw(rect, mColor * color);
	}

	void ColoredImage::draw(grect rect, April::Color color, float angle)
	{
		Image::draw(rect, mColor * color, angle);
	}

	void ColoredImage::setColor(float a, float r, float g, float b)
	{
		mColor.setColor(a, r, g, b);
	}

}

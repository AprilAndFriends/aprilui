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

#include "ColoredImage.h"

namespace AprilUI
{
	ColoredImage::ColoredImage(April::Texture* texture, chstr name, float sx, float sy, float sw, float sh, bool vertical, unsigned int color) :
				  Image(texture, name, sx, sy, sw, sh, vertical)
	{
		mColor.setColor(color);
	}

	float ColoredImage::getAlpha()
	{
		return mColor.a_float();
	}
	
	void ColoredImage::setAlpha(float alpha)
	{
		mColor.a = alpha * 255.0f;
	}
		
	void ColoredImage::draw(float x, float y, float w, float h, float r, float g, float b, float a)
	{
		Image::draw(x, y, w, h, mColor.r_float() * r, mColor.r_float() * r, mColor.r_float() * r, mColor.r_float() * r);
	}

	void ColoredImage::draw(float x, float y, float w, float h, float angle, float r, float g, float b, float a)
	{
		Image::draw(x, y, w, h, angle, mColor.r_float() * r, mColor.r_float() * r, mColor.r_float() * r, mColor.r_float() * r);
	}

	void ColoredImage::setColor(float a, float r, float g, float b)
	{
		mColor.setColor(a, r, g, b);
	}

}

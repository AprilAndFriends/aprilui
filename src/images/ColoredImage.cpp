/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>

#include "ColoredImage.h"
#include "Texture.h"

namespace aprilui
{
	ColoredImage::ColoredImage(Texture* texture, chstr name, grectf source, bool vertical, april::Color color) :
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
		mColor.a = (unsigned char)(alpha * 255);
	}
	
	void ColoredImage::draw(grectf rect, april::Color color)
	{
		Image::draw(rect, mColor * color);
	}

	void ColoredImage::draw(grectf rect, april::Color color, float angle, gvec2f center)
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

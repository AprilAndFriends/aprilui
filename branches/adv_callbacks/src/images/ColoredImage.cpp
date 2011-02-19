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

	ColoredImage::ColoredImage(Image& img,chstr name,april::Color color) : Image(img,name)
	{
		mColor=color;
	}

	unsigned char ColoredImage::getAlpha()
	{
		return mColor.a;
	}
	
	void ColoredImage::setAlpha(unsigned char value)
	{
		mColor.a = value;
	}
	
	void ColoredImage::draw(grect rect, april::Color color)
	{
		Image::draw(rect, mColor * color);
	}

}

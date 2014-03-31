/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 3.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>

#include "ColoredImage.h"
#include "Texture.h"

namespace aprilui
{
	ColoredImage::ColoredImage(Texture* texture, chstr name, grect source, bool vertical, bool invertX, bool invertY, april::Color color) :
				  Image(texture, name, source, vertical, invertX, invertY)
	{
		this->color = color;
	}

	ColoredImage::ColoredImage(Image& img, chstr name, april::Color color) : Image(img, name)
	{
		this->color = color;
	}

	ColoredImage::~ColoredImage()
	{
	}

	void ColoredImage::draw(grect rect, april::Color color)
	{
		Image::draw(rect, this->color * color);
	}

}

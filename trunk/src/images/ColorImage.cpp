/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.52
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <april/RenderSystem.h>
#include <gtypes/Matrix4.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>

#include "apriluiUtil.h"
#include "ColorImage.h"

namespace aprilui
{
	april::PlainVertex pVertices[4];
	
	ColorImage::ColorImage(chstr name) : Image(0, name, grect())
	{
		mColor.set(name);
	}

	ColorImage::ColorImage(chstr name, grect rect) : Image(0, name, rect)
	{
		mColor.set(name);
	}

	ColorImage::ColorImage(chstr name, grect rect, chstr color) : Image(0, name, rect)
	{
		mColor.set(color);
	}

	ColorImage::ColorImage(chstr name, grect rect, april::Color color) : Image(0, name, rect)
	{
		mColor = color;
	}

	ColorImage::~ColorImage()
	{
	}

	void ColorImage::draw(grect rect, april::Color color)
	{
		pVertices[0].x = rect.x;          pVertices[0].y = rect.y;
		pVertices[1].x = rect.x + rect.w; pVertices[1].y = rect.y;
		pVertices[2].x = rect.x;          pVertices[2].y = rect.y + rect.h;
		pVertices[3].x = rect.x + rect.w; pVertices[3].y = rect.y + rect.h;
		
		april::rendersys->setTextureBlendMode(mBlendMode);
		april::rendersys->render(april::TriangleStrip, pVertices, 4, color * mColor);
		april::rendersys->setTextureBlendMode(april::DEFAULT);
	}

}

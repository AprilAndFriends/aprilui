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
#include <gtypes/Matrix4.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>

#include "ColorImage.h"
#include "Util.h"

namespace aprilui
{
	april::PlainVertex pVertices[4];
	
	ColorImage::ColorImage(chstr name) : Image(0, name, grect())
	{
		mColor.set(name);
	}

	void ColorImage::draw(grect rect, april::Color color)
	{
		april::PlainVertex v[4];
		pVertices[0].x = rect.x;          pVertices[0].y = rect.y;
		pVertices[1].x = rect.x + rect.w; pVertices[1].y = rect.y;
		pVertices[2].x = rect.x;          pVertices[2].y = rect.y + rect.h;
		pVertices[3].x = rect.x + rect.w; pVertices[3].y = rect.y + rect.h;
		
		if (mBlendMode != april::ALPHA_BLEND)
		{
			april::rendersys->setBlendMode(mBlendMode);
		}
		color *= mColor;
		april::rendersys->render(april::TriangleStrip, pVertices, 4, color);
		if (mBlendMode != april::ALPHA_BLEND)
		{
			april::rendersys->setBlendMode(april::DEFAULT);
		}
	}

	void ColorImage::draw(grect rect, april::Color color, float angle, gvec2 center)
	{
		pVertices[0].x = -center.x;			pVertices[0].y = -center.y;
		pVertices[1].x = rect.w - center.x;	pVertices[1].y = -center.y;
		pVertices[2].x = -center.x;			pVertices[2].y = rect.h - center.y;
		pVertices[3].x = rect.w - center.x;	pVertices[3].y = rect.h - center.y;
		
		gtypes::Matrix4 originalMatrix = april::rendersys->getModelviewMatrix();
		april::rendersys->setIdentityTransform();
		april::rendersys->translate(rect.x + center.x, rect.y + center.y);
		april::rendersys->rotate(angle);
		if (mBlendMode != april::ALPHA_BLEND)
		{
			april::rendersys->setBlendMode(mBlendMode);
		}
		april::rendersys->render(april::TriangleStrip, pVertices, 4, color);
		if (mBlendMode != april::ALPHA_BLEND)
		{
			april::rendersys->setBlendMode(april::DEFAULT);
		}
		april::rendersys->setModelviewMatrix(originalMatrix);
	}
	
}

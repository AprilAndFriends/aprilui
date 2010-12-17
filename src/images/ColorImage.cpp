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
	April::PlainVertex pVertices[4];
	
	ColorImage::ColorImage(chstr name) : Image(0, name, grect())
	{
		mColor.set(name);
	}

	void ColorImage::draw(grect rect, April::Color color)
	{
		April::PlainVertex v[4];
		pVertices[0].x = rect.x;          pVertices[0].y = rect.y;
		pVertices[1].x = rect.x + rect.w; pVertices[1].y = rect.y;
		pVertices[2].x = rect.x;          pVertices[2].y = rect.y + rect.h;
		pVertices[3].x = rect.x + rect.w; pVertices[3].y = rect.y + rect.h;
		
		if (mBlendMode != April::ALPHA_BLEND)
		{
			April::rendersys->setBlendMode(mBlendMode);
		}
		color *= mColor;
		April::rendersys->render(April::TriangleStrip, pVertices, 4, color.r_float(), color.g_float(), color.b_float(), color.a_float());
		if (mBlendMode != April::ALPHA_BLEND)
		{
			April::rendersys->setBlendMode(April::DEFAULT);
		}
	}

	void ColorImage::draw(grect rect, April::Color color, float angle, gvec2 center)
	{
		pVertices[0].x = -center.x;			pVertices[0].y = -center.y;
		pVertices[1].x = rect.w - center.x;	pVertices[1].y = -center.y;
		pVertices[2].x = -center.x;			pVertices[2].y = rect.h - center.y;
		pVertices[3].x = rect.w - center.x;	pVertices[3].y = rect.h - center.y;
		
		gtypes::Matrix4 originalMatrix = April::rendersys->getModelviewMatrix();
		April::rendersys->setIdentityTransform();
		April::rendersys->translate(rect.x + center.x, rect.y + center.y);
		April::rendersys->rotate(angle);
		if (mBlendMode != April::ALPHA_BLEND)
		{
			April::rendersys->setBlendMode(mBlendMode);
		}
		April::rendersys->render(April::TriangleStrip, pVertices, 4, color.r_float(), color.g_float(), color.b_float(), color.a_float());
		if (mBlendMode != April::ALPHA_BLEND)
		{
			April::rendersys->setBlendMode(April::DEFAULT);
		}
		April::rendersys->setModelviewMatrix(originalMatrix);
	}
	
}

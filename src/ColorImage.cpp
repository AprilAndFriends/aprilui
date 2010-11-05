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

#include "ColorImage.h"
#include "Util.h"

namespace AprilUI
{
	ColorImage::ColorImage(chstr name) : Image(0, name, grect())
	{
		mColor.setColor(name);
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

	void ColorImage::draw(grect rect, April::Color color, float angle)
	{
		pVertices[0].x = -rect.w / 2; pVertices[0].y = -rect.h / 2;
		pVertices[1].x =  rect.w / 2; pVertices[1].y = -rect.h / 2;
		pVertices[2].x = -rect.w / 2; pVertices[2].y =  rect.h / 2;
		pVertices[3].x =  rect.w / 2; pVertices[3].y =  rect.h / 2;
		
		gtypes::Matrix4 temp_matrix = April::rendersys->getModelviewMatrix();
		April::rendersys->setIdentityTransform();
		April::rendersys->translate(rect.x, rect.y);
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
		April::rendersys->setModelviewMatrix(temp_matrix);
	}

}

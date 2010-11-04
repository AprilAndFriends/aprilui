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

	void ColorImage::draw(float x, float y, float w, float h, float r, float g, float b, float a)
	{
		April::PlainVertex v[4];
		pVertices[0].x = x;     pVertices[0].y = y;
		pVertices[1].x = x + w; pVertices[1].y = y;
		pVertices[2].x = x;     pVertices[2].y = y + h;
		pVertices[3].x = x + w; pVertices[3].y = y + h;
		
		if (mBlendMode != April::ALPHA_BLEND)
		{
			April::rendersys->setBlendMode(mBlendMode);
		}
		April::rendersys->render(April::TriangleStrip, v, 4, mColor.r_float() * r,
			mColor.g_float() * g, mColor.b_float() * b, mColor.a_float() * a);
		if (mBlendMode != April::ALPHA_BLEND)
		{
			April::rendersys->setBlendMode(April::DEFAULT);
		}
	}

	void ColorImage::draw(float x, float y, float w, float h, float angle, float r, float g, float b, float a)
	{
		pVertices[0].x = -w / 2; pVertices[0].y = -h / 2;
		pVertices[1].x =  w / 2; pVertices[1].y = -h / 2;
		pVertices[2].x = -w / 2; pVertices[2].y =  h / 2;
		pVertices[3].x =  w / 2; pVertices[3].y =  h / 2;
		
		gtypes::Matrix4 temp_matrix = April::rendersys->getModelviewMatrix();
		April::rendersys->setIdentityTransform();
		April::rendersys->translate(x, y);
		April::rendersys->rotate(angle);
		if (mBlendMode != April::ALPHA_BLEND)
		{
			April::rendersys->setBlendMode(mBlendMode);
		}
		April::rendersys->render(April::TriangleStrip,pVertices, 4, mColor.r_float() * r,
			mColor.g_float() * g, mColor.b_float() * b, mColor.a_float() * a);
		if (mBlendMode != April::ALPHA_BLEND)
		{
			April::rendersys->setBlendMode(April::DEFAULT);
		}
		April::rendersys->setModelviewMatrix(temp_matrix);
	}

}

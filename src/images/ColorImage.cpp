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
	ColorImage::ColorImage(chstr name) : Image(0, name, grect())
	{
		mColor.set(name);
	}

	void ColorImage::draw(grect rect, april::Color color)
	{
		_pVertices[0].x = rect.x;          _pVertices[0].y = rect.y;
		_pVertices[1].x = rect.x + rect.w; _pVertices[1].y = rect.y;
		_pVertices[2].x = rect.x;          _pVertices[2].y = rect.y + rect.h;
		_pVertices[3].x = rect.x + rect.w; _pVertices[3].y = rect.y + rect.h;
		
		if (mBlendMode != april::ALPHA_BLEND)
		{
			april::rendersys->setTextureBlendMode(mBlendMode);
		}
		color *= mColor;
		april::rendersys->render(april::TriangleStrip, _pVertices, 4, color);
		if (mBlendMode != april::ALPHA_BLEND)
		{
			april::rendersys->setTextureBlendMode(april::DEFAULT);
		}
	}

	void ColorImage::draw(grect rect, april::Color color, float angle, gvec2 center)
	{
		_pVertices[0].x = -center.x;			_pVertices[0].y = -center.y;
		_pVertices[1].x = rect.w - center.x;	_pVertices[1].y = -center.y;
		_pVertices[2].x = -center.x;			_pVertices[2].y = rect.h - center.y;
		_pVertices[3].x = rect.w - center.x;	_pVertices[3].y = rect.h - center.y;
		
		gtypes::Matrix4 temp_matrix = april::rendersys->getModelviewMatrix();
		april::rendersys->setIdentityTransform();
		april::rendersys->translate(rect.x + center.x, rect.y + center.y);
		april::rendersys->rotate(angle);
		if (mBlendMode != april::ALPHA_BLEND)
		{
			april::rendersys->setTextureBlendMode(mBlendMode);
		}
		april::rendersys->render(april::TriangleStrip, _pVertices, 4, color);
		if (mBlendMode != april::ALPHA_BLEND)
		{
			april::rendersys->setTextureBlendMode(april::DEFAULT);
		}
		april::rendersys->setModelviewMatrix(temp_matrix);
	}

}

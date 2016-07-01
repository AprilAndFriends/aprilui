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

#include "apriluiUtil.h"
#include "ColorImage.h"

namespace aprilui
{
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

	void ColorImage::draw(grect rect, april::Color color)
	{
		_pVertices[0].x = rect.x;          _pVertices[0].y = rect.y;
		_pVertices[1].x = rect.x + rect.w; _pVertices[1].y = rect.y;
		_pVertices[2].x = rect.x;          _pVertices[2].y = rect.y + rect.h;
		_pVertices[3].x = rect.x + rect.w; _pVertices[3].y = rect.y + rect.h;
		
		april::rendersys->setBlendMode(mBlendMode);
		april::rendersys->setColorMode(april::CM_DEFAULT);
		color *= mColor;
		april::rendersys->render(april::RO_TRIANGLE_STRIP, _pVertices, 4, color);
		april::rendersys->setBlendMode(april::BM_DEFAULT);
		april::rendersys->setColorMode(april::CM_DEFAULT);
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
		april::rendersys->setBlendMode(mBlendMode);
		april::rendersys->setColorMode(april::CM_DEFAULT);
		april::rendersys->render(april::RO_TRIANGLE_STRIP, _pVertices, 4, color);
		april::rendersys->setBlendMode(april::BM_DEFAULT);
		april::rendersys->setColorMode(april::CM_DEFAULT);
		april::rendersys->setModelviewMatrix(temp_matrix);
	}

}

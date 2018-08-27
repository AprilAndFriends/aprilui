/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/RenderSystem.h>
#include <gtypes/Matrix4.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>

#include "apriluiUtil.h"
#include "ColorImage.h"

namespace aprilui
{
	ColorImage::ColorImage(chstr name) : Image(0, name, grectf())
	{
		mColor.set(name);
	}

	ColorImage::ColorImage(chstr name, grectf rect) : Image(0, name, rect)
	{
		mColor.set(name);
	}

	ColorImage::ColorImage(chstr name, grectf rect, chstr color) : Image(0, name, rect)
	{
		mColor.set(color);
	}

	ColorImage::ColorImage(chstr name, grectf rect, april::Color color) : Image(0, name, rect)
	{
		mColor = color;
	}

	void ColorImage::draw(grectf rect, april::Color color)
	{
		_pVertices[0].x = rect.x;          _pVertices[0].y = rect.y;
		_pVertices[1].x = rect.x + rect.w; _pVertices[1].y = rect.y;
		_pVertices[2].x = rect.x;          _pVertices[2].y = rect.y + rect.h;
		_pVertices[3].x = rect.x + rect.w; _pVertices[3].y = rect.y + rect.h;
		
		april::rendersys->setBlendMode(mBlendMode);
		april::rendersys->setColorMode(april::ColorMode::Multiply);
		color *= mColor;
		april::rendersys->render(april::RenderOperation::TriangleStrip, _pVertices, 4, color);
		april::rendersys->setBlendMode(april::BlendMode::Alpha);
		april::rendersys->setColorMode(april::ColorMode::Multiply);
	}

	void ColorImage::draw(grectf rect, april::Color color, float angle, gvec2f center)
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
		april::rendersys->setColorMode(april::ColorMode::Multiply);
		april::rendersys->render(april::RenderOperation::TriangleStrip, _pVertices, 4, color);
		april::rendersys->setBlendMode(april::BlendMode::Alpha);
		april::rendersys->setColorMode(april::ColorMode::Multiply);
		april::rendersys->setModelviewMatrix(temp_matrix);
	}

}

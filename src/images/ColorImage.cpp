/// @file
/// @version 5.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hlog.h>
#include <hltypes/hltypesUtil.h>

#include "aprilui.h"
#include "apriluiUtil.h"
#include "ColorImage.h"

namespace aprilui
{
	ColorImage::ColorImage(chstr name, const harray<april::Color>& colors) :
		BaseImage(name)
	{
		this->colorTopLeft = colors[0];
		if (colors.size() == 4)
		{
			this->colorTopRight = colors[1];
			this->colorBottomLeft = colors[2];
			this->colorBottomRight = colors[3];
		}
		else
		{
			this->colorTopRight = colors[0];
			this->colorBottomLeft = colors[0];
			this->colorBottomRight = colors[0];
		}
	}

	ColorImage::ColorImage(const ColorImage& other) :
		BaseImage(other)
	{
		this->colorTopLeft = other.colorTopLeft;
		this->colorTopRight = other.colorTopRight;
		this->colorBottomLeft = other.colorBottomLeft;
		this->colorBottomRight = other.colorBottomRight;
	}
	
	void ColorImage::draw(cgrectf rect, const april::Color& color)
	{
		if (color.a == 0)
		{
			return;
		}
		april::Color topLeft = this->colorTopLeft * color;
		if (topLeft.a == 0)
		{
			return;
		}
		april::Color topRight = this->colorTopRight * color;
		if (topRight.a == 0)
		{
			return;
		}
		april::Color bottomLeft = this->colorBottomLeft * color;
		if (bottomLeft.a == 0)
		{
			return;
		}
		april::Color bottomRight = this->colorBottomRight * color;
		if (bottomRight.a == 0)
		{
			return;
		}
		this->vertices[0].x = this->vertices[2].x = this->vertices[4].x = rect.left();
		this->vertices[0].y = this->vertices[1].y = this->vertices[3].y = rect.top();
		this->vertices[1].x = this->vertices[3].x = this->vertices[5].x = rect.right();
		this->vertices[2].y = this->vertices[4].y = this->vertices[5].y = rect.bottom();
		this->vertices[0].color = april::rendersys->getNativeColorUInt(topLeft);
		this->vertices[1].color = this->vertices[3].color = april::rendersys->getNativeColorUInt(topRight);
		this->vertices[2].color = this->vertices[4].color = april::rendersys->getNativeColorUInt(bottomLeft);
		this->vertices[5].color = april::rendersys->getNativeColorUInt(bottomRight);
		april::rendersys->setBlendMode(april::BlendMode::Alpha);
		april::rendersys->setColorMode(april::ColorMode::Multiply, 1.0f);
		april::rendersys->render(april::RenderOperation::TriangleList, this->vertices, APRILUI_COLOR_MAX_VERTICES);
	}

	void ColorImage::draw(const harray<april::TexturedVertex>& vertices, const april::Color& color)
	{
		hlog::warn(logTag, "ColorImage::draw(harray<april::TexturedVertex>, april::Color) is not supported!");
	}
	
}

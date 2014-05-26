/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/RenderSystem.h>
#include <gtypes/Matrix4.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hlog.h>

#include "aprilui.h"
#include "apriluiUtil.h"
#include "ColorImage.h"

namespace aprilui
{
	ColorImage::ColorImage(chstr name) : Image(0, name, grect())
	{
		hlog::warn(aprilui::logTag, "'ColorImage' is deprecated, use 'ColoredQuad' instead!"); // DEPRECATED
		this->color.set(name);
	}

	ColorImage::ColorImage(chstr name, grect rect) : Image(0, name, rect)
	{
		hlog::warn(aprilui::logTag, "'ColorImage' is deprecated, use 'ColoredQuad' instead!"); // DEPRECATED
		this->color.set(name);
	}

	ColorImage::ColorImage(chstr name, grect rect, chstr color) : Image(0, name, rect)
	{
		hlog::warn(aprilui::logTag, "'ColorImage' is deprecated, use 'ColoredQuad' instead!"); // DEPRECATED
		this->color.set(color);
	}

	ColorImage::ColorImage(chstr name, grect rect, april::Color color) : Image(0, name, rect)
	{
		hlog::warn(aprilui::logTag, "'ColorImage' is deprecated, use 'ColoredQuad' instead!"); // DEPRECATED
		this->color = color;
	}

	ColorImage::~ColorImage()
	{
	}

	void ColorImage::draw(grect rect, april::Color color)
	{
		this->_pVertices[0].x = this->_pVertices[2].x = rect.left();
		this->_pVertices[0].y = this->_pVertices[1].y = rect.top();
		this->_pVertices[1].x = this->_pVertices[3].x = rect.right();
		this->_pVertices[2].y = this->_pVertices[3].y = rect.bottom();
		
		april::rendersys->setTextureBlendMode(this->blendMode);
		april::rendersys->render(april::RO_TRIANGLE_STRIP, this->_pVertices, 4, this->color * color);
		april::rendersys->setTextureBlendMode(april::BM_DEFAULT);
	}

	void ColorImage::draw(harray<april::TexturedVertex> vertices, april::Color color)
	{
		harray<april::PlainVertex> plainVertices;
		foreach (april::TexturedVertex, it, vertices)
		{
			plainVertices += april::PlainVertex(*it);
		}
		april::rendersys->setTextureBlendMode(this->blendMode);
		april::rendersys->render(april::RO_TRIANGLE_LIST, &plainVertices[0], plainVertices.size(), color);
		april::rendersys->setTextureBlendMode(april::BM_DEFAULT);
	}
	
}

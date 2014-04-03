/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 3.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <math.h>

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hltypesUtil.h>

#include "apriluiUtil.h"
#include "Dataset.h"
#include "Exception.h"
#include "Image.h"
#include "Texture.h"

namespace aprilui
{
	Image::Image(Texture* texture, chstr name, grect source, bool vertical, bool invertX, bool invertY)
	{
		this->texture = texture;
		this->name = name;
		this->srcRect = source;
		this->blendMode = april::BM_DEFAULT;
		this->colorMode = april::CM_DEFAULT;
		this->colorModeFactor = 1.0f;
		this->vertical = vertical;
		this->invertX = invertX;
		this->invertY = invertY;
		this->_textureCoordinatesLoaded = false;
		this->dataset = NULL;
	}

	Image::Image(Image& img, chstr name)
	{
		this->texture = img.texture;
		this->name = name;
		this->srcRect = img.srcRect;
		this->blendMode = img.blendMode;
		this->colorMode = img.colorMode;
		this->colorModeFactor = img.colorModeFactor;
		this->vertical = img.vertical;
		this->invertX = img.invertX;
		this->invertY = img.invertY;
		this->_textureCoordinatesLoaded = false;
	}
	
	Image::~Image()
	{
	}
	
	hstr Image::getFullName()
	{
		return (this->dataset != NULL ? this->dataset->getName() + "." + this->name : this->name);
	}
	
	void Image::setSrcRect(grect value)
	{
		this->srcRect = value;
		this->_textureCoordinatesLoaded = false;
	}
	
	void Image::_tryLoadTexCoords()
	{
		if (!this->_textureCoordinatesLoaded && this->texture != NULL && this->texture->getWidth() > 0 && this->texture->getHeight() > 0)
		{
			this->_textureCoordinatesLoaded = true;
			float iw = 1.0f / this->texture->getWidth();
			float ih = 1.0f / this->texture->getHeight();

			this->_tVertices[0].u = this->_tVertices[2].u = this->srcRect.left() * iw;
			this->_tVertices[0].v = this->_tVertices[1].v = this->srcRect.top() * ih;
			this->_tVertices[1].u = this->_tVertices[3].u = this->srcRect.right() * iw;
			this->_tVertices[2].v = this->_tVertices[3].v = this->srcRect.bottom() * ih;
			if (this->invertX)
			{
				hswap(this->_tVertices[0].u, this->_tVertices[1].u);
				hswap(this->_tVertices[2].u, this->_tVertices[3].u);
			}
			if (this->invertY)
			{
				hswap(this->_tVertices[0].v, this->_tVertices[2].v);
				hswap(this->_tVertices[1].v, this->_tVertices[3].v);
			}
			// vertical is applied last
			if (this->vertical)
			{
				hswap(this->_tVertices[0].u, this->_tVertices[2].v);
				hswap(this->_tVertices[3].u, this->_tVertices[1].v);
			}
		}
	}

	void Image::draw(grect rect, april::Color color)
	{
		this->_tVertices[0].x = this->_tVertices[2].x = rect.left();
		this->_tVertices[0].y = this->_tVertices[1].y = rect.top();
		this->_tVertices[1].x = this->_tVertices[3].x = rect.right();
		this->_tVertices[2].y = this->_tVertices[3].y = rect.bottom();
		
		this->texture->load();
		april::rendersys->setTexture(this->texture->getRenderTexture());
		this->_tryLoadTexCoords();
		
		april::rendersys->setTextureBlendMode(this->blendMode);
		april::rendersys->setTextureColorMode(this->colorMode, this->colorModeFactor);
		if (color.r < 255 || color.g < 255 || color.b < 255 || color.a < 255)
		{
			april::rendersys->render(april::RO_TRIANGLE_STRIP, this->_tVertices, 4, color);
		}
		else
		{
			april::rendersys->render(april::RO_TRIANGLE_STRIP, this->_tVertices, 4);
		}
		april::rendersys->setTextureBlendMode(april::BM_DEFAULT);
		april::rendersys->setTextureColorMode(april::CM_DEFAULT);
	}

	void Image::draw(harray<april::TexturedVertex> vertices, april::Color color)
	{
		this->texture->load();
		april::rendersys->setTexture(this->texture->getRenderTexture());
		this->_tryLoadTexCoords();

		// texture coordinate scaling
		float iw = 1.0f / this->texture->getWidth();
		float ih = 1.0f / this->texture->getHeight();
		foreach (april::TexturedVertex, it, vertices)
		{
			it->u = (this->srcRect.x + it->u * this->srcRect.w) * iw;
			it->v = (this->srcRect.y + it->v * this->srcRect.h) * ih;
		}

		april::rendersys->setTextureBlendMode(this->blendMode);
		april::rendersys->setTextureColorMode(this->colorMode, this->colorModeFactor);
		if (color.r < 255 || color.g < 255 || color.b < 255 || color.a < 255)
		{
			april::rendersys->render(april::RO_TRIANGLE_LIST, &vertices[0], vertices.size(), color);
		}
		else
		{
			april::rendersys->render(april::RO_TRIANGLE_LIST, &vertices[0], vertices.size());
		}
		april::rendersys->setTextureBlendMode(april::BM_DEFAULT);
		april::rendersys->setTextureColorMode(april::CM_DEFAULT);
	}
	
}

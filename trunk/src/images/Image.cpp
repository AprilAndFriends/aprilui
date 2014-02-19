/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

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
		this->colorModeAlpha = 255;
		this->vertical = vertical;
		this->invertedX = invertX;
		this->invertedY = invertY;
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
		this->colorModeAlpha = img.colorModeAlpha;
		this->vertical = img.vertical;
		this->invertedX = img.invertedX;
		this->invertedY = img.invertedY;
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
			if (!this->vertical)
			{
				this->_tVertices[0].u = this->_tVertices[2].u = this->srcRect.left() * iw;
				this->_tVertices[0].v = this->_tVertices[1].v = this->srcRect.top() * ih;
				this->_tVertices[1].u = this->_tVertices[3].u = this->srcRect.right() * iw;
				this->_tVertices[2].v = this->_tVertices[3].v = this->srcRect.bottom() * ih;
				if (this->invertedX)
				{
					hswap(this->_tVertices[0].u, this->_tVertices[1].u);
					hswap(this->_tVertices[2].u, this->_tVertices[3].u);
				}
				if (this->invertedY)
				{
					hswap(this->_tVertices[0].v, this->_tVertices[2].v);
					hswap(this->_tVertices[1].v, this->_tVertices[3].v);
				}
			}
			else
			{
				this->_tVertices[0].u = this->_tVertices[1].u = (this->srcRect.x + this->srcRect.h) * iw;
				this->_tVertices[0].v = this->_tVertices[2].v = this->srcRect.y * ih;
				this->_tVertices[1].v = this->_tVertices[3].v = (this->srcRect.y + this->srcRect.w) * ih;
				this->_tVertices[2].u = this->_tVertices[3].u = this->srcRect.x * iw;
				if (this->invertedY)
				{
					hswap(this->_tVertices[0].u, this->_tVertices[2].u);
					hswap(this->_tVertices[1].u, this->_tVertices[3].u);
				}
				if (this->invertedX)
				{
					hswap(this->_tVertices[0].v, this->_tVertices[1].v);
					hswap(this->_tVertices[2].v, this->_tVertices[3].v);
				}
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
		april::rendersys->setTextureColorMode(this->colorMode, this->colorModeAlpha);
		if (color.r < 255 || color.g < 255 || color.b < 255 || color.a < 255)
		{
			april::rendersys->render(april::RO_TRIANGLE_STRIP, this->_tVertices, 4, color);
		}
		else
		{
			april::rendersys->render(april::RO_TRIANGLE_STRIP, this->_tVertices, 4);
		}
		april::rendersys->setTextureBlendMode(april::BM_DEFAULT);
		april::rendersys->setTextureColorMode(april::CM_DEFAULT, 255);
	}
	
	void Image::draw(grect rect, april::Color color, float angle)
	{
		if (angle == 0.0f)
		{
			this->draw(rect, color);
			return;
		}
		gmat4 originalMatrix = april::rendersys->getModelviewMatrix();
		april::rendersys->translate(rect.w * 0.5f, rect.h * 0.5f);
		april::rendersys->rotate(angle);
		this->draw(rect - rect.getSize() * 0.5f, color);
		april::rendersys->setModelviewMatrix(originalMatrix);
	}

}

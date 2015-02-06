/// @file
/// @version 4.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>

#include "apriluiUtil.h"
#include "Texture.h"
#include "TileImage.h"

namespace aprilui
{
	harray<PropertyDescription> TileImage::_propertyDescriptions;

	TileImage::TileImage(Texture* texture, chstr name, grect source) : Image(texture, name, source)
	{
		this->tile = source.getSize();
		this->useTileCount = false;
	}

	TileImage::~TileImage()
	{
	}

	harray<PropertyDescription> TileImage::getPropertyDescriptions()
	{
		if (TileImage::_propertyDescriptions.size() == 0)
		{
			TileImage::_propertyDescriptions += PropertyDescription("tile", PropertyDescription::GVEC2);
			TileImage::_propertyDescriptions += PropertyDescription("tile_w", PropertyDescription::FLOAT);
			TileImage::_propertyDescriptions += PropertyDescription("tile_h", PropertyDescription::FLOAT);
			TileImage::_propertyDescriptions += PropertyDescription("scroll", PropertyDescription::GVEC2);
			TileImage::_propertyDescriptions += PropertyDescription("scroll_x", PropertyDescription::FLOAT);
			TileImage::_propertyDescriptions += PropertyDescription("scroll_y", PropertyDescription::FLOAT);
			TileImage::_propertyDescriptions += PropertyDescription("use_tile_count", PropertyDescription::BOOL);
		}
		return (Image::getPropertyDescriptions() + TileImage::_propertyDescriptions);
	}

	hstr TileImage::getProperty(chstr name)
	{
		if (name == "tile")				return april::gvec2ToHstr(this->getTile());
		if (name == "tile_w")			return this->getTileW();
		if (name == "tile_h")			return this->getTileH();
		if (name == "scroll")			return april::gvec2ToHstr(this->getScroll());
		if (name == "scroll_x")			return this->getScrollX();
		if (name == "scroll_y")			return this->getScrollY();
		if (name == "use_tile_count")	return this->isUseTileCount();
		return Image::getProperty(name);
	}

	bool TileImage::setProperty(chstr name, chstr value)
	{
		if		(name == "tile")			this->setTile(april::hstrToGvec2(value));
		else if	(name == "tile_w")			this->setTileW(value);
		else if (name == "tile_h")			this->setTileH(value);
		else if	(name == "scroll")			this->setScroll(april::hstrToGvec2(value));
		else if (name == "scroll_x")		this->setScrollX(value);
		else if	(name == "scroll_y")		this->setScrollY(value);
		else if (name == "use_tile_count")	this->setUseTileCount(value);
		else return Image::setProperty(name, value);
		return true;
	}

	void TileImage::_createVertices(grect rect)
	{
		this->tileVertices.clear();
		gvec2 positions[2] = { rect.getTopLeft(), rect.getBottomRight() };
		gvec2 srcs[2];
		gvec2 scroll = this->scroll;
		gvec2 tile = this->tile;
		april::TexturedVertex v;
		april::Texture* renderTexture = this->texture->getTexture();
		gvec2 srcSize = this->srcRect.getSize();
		if (this->rotated)
		{
			hswap(srcSize.x, srcSize.y);
		}
		bool fullTexture = (this->texture->isValid() && renderTexture->isLoaded() &&
			renderTexture->getAddressMode() == april::Texture::ADDRESS_WRAP &&
			this->srcRect.x == 0.0f && this->srcRect.y == 0.0f &&
			srcSize.x == (float)this->texture->getWidth() && srcSize.y == (float)this->texture->getHeight());
		if (fullTexture)
		{
			if (this->rotated)
			{
				hswap(rect.w, rect.h);
				float y = scroll.y;
				scroll.y = scroll.x;
				scroll.x = -y;
			}
			if (!this->useTileCount)
			{
				tile = rect.getSize() / tile;
			}
			srcs[0] = -scroll / rect.getSize() * tile;
			srcs[1] = tile + srcs[0];
			if (!this->rotated)
			{
				v.x = positions[0].x;	v.y = positions[0].y;	v.u = srcs[0].x;	v.v = srcs[0].y;	this->tileVertices += v;
				v.x = positions[1].x;	v.y = positions[0].y;	v.u = srcs[1].x;	v.v = srcs[0].y;	this->tileVertices += v;
				v.x = positions[0].x;	v.y = positions[1].y;	v.u = srcs[0].x;	v.v = srcs[1].y;	this->tileVertices += v;
				this->tileVertices += this->tileVertices(-2, 2);
				v.x = positions[1].x;	v.y = positions[1].y;	v.u = srcs[1].x;	v.v = srcs[1].y;	this->tileVertices += v;
			}
			else
			{
				v.x = positions[0].x;	v.y = positions[0].y;	v.u = srcs[1].x;	v.v = srcs[0].y;	this->tileVertices += v;
				v.x = positions[1].x;	v.y = positions[0].y;	v.u = srcs[1].x;	v.v = srcs[1].y;	this->tileVertices += v;
				v.x = positions[0].x;	v.y = positions[1].y;	v.u = srcs[0].x;	v.v = srcs[0].y;	this->tileVertices += v;
				this->tileVertices += this->tileVertices(-2, 2);
				v.x = positions[1].x;	v.y = positions[1].y;	v.u = srcs[0].x;	v.v = srcs[1].y;	this->tileVertices += v;
			}
		}
		else
		{
			if (this->useTileCount)
			{
				tile = rect.getSize() / tile;
			}
			gvec2 invSize(1.0f / this->texture->getWidth(), 1.0f / this->texture->getHeight());
			if (this->rotated)
			{
				hswap(invSize.x, invSize.y);
			}
			grect invSrcRect(this->srcRect.getPosition() * invSize, this->srcRect.getSize() * invSize);
			if (this->rotated)
			{
				hswap(invSrcRect.w, invSrcRect.h);
			}
			gvec2 srcFactor = invSrcRect.getSize() / tile;
			float difference = 0.0f;
			scroll.x = hmodf(this->scroll.x, tile.x) - tile.x;
			scroll.y = hmodf(this->scroll.y, tile.y) - tile.y;
			int countX = hceil((rect.w - scroll.x) / tile.x);
			int countY = hceil((rect.h - scroll.y) / tile.y);
			int i;
			int j;
			if (!this->rotated)
			{
				for_iterx (j, 0, countY)
				{
					for_iterx (i, 0, countX)
					{
						positions[0] = rect.getPosition() + scroll + gvec2((float)i, (float)j) * tile;
						positions[1] = positions[0] + tile;
						srcs[0] = invSrcRect.getTopLeft();
						srcs[1] = invSrcRect.getBottomRight();
						difference = rect.x - positions[0].x;
						if (difference > 0.0f)
						{
							srcs[0].x += difference * srcFactor.x;
							positions[0].x += difference;
						}
						difference = positions[1].x - (rect.x + rect.w);
						if (difference > 0.0f)
						{
							srcs[1].x -= difference * srcFactor.x;
							positions[1].x -= difference;
						}
						difference = rect.y - positions[0].y;
						if (difference > 0.0f)
						{
							srcs[0].y += difference * srcFactor.y;
							positions[0].y += difference;
						}
						difference = positions[1].y - (rect.y + rect.h);
						if (difference > 0.0f)
						{
							srcs[1].y -= difference * srcFactor.y;
							positions[1].y -= difference;
						}
						v.x = positions[0].x;	v.y = positions[0].y;	v.u = srcs[0].x;	v.v = srcs[0].y;	this->tileVertices += v;
						v.x = positions[1].x;	v.y = positions[0].y;	v.u = srcs[1].x;	v.v = srcs[0].y;	this->tileVertices += v;
						v.x = positions[0].x;	v.y = positions[1].y;	v.u = srcs[0].x;	v.v = srcs[1].y;	this->tileVertices += v;
						this->tileVertices += this->tileVertices(-2, 2);
						v.x = positions[1].x;	v.y = positions[1].y;	v.u = srcs[1].x;	v.v = srcs[1].y;	this->tileVertices += v;
					}
				}
			}
			else
			{
				for_iterx (j, 0, countY)
				{
					for_iterx (i, 0, countX)
					{
						positions[0] = rect.getPosition() + scroll + gvec2((float)i, (float)j) * tile;
						positions[1] = positions[0] + tile;
						srcs[0] = invSrcRect.getTopLeft();
						srcs[1] = invSrcRect.getBottomRight();
						difference = rect.x - positions[0].x;
						if (difference > 0.0f)
						{
							srcs[0].y += difference * srcFactor.x;
							positions[0].x += difference;
						}
						difference = positions[1].x - (rect.x + rect.w);
						if (difference > 0.0f)
						{
							srcs[1].y -= difference * srcFactor.x;
							positions[1].x -= difference;
						}
						difference = rect.y - positions[0].y;
						if (difference > 0.0f)
						{
							srcs[1].x -= difference * srcFactor.y;
							positions[0].y += difference;
						}
						difference = positions[1].y - (rect.y + rect.h);
						if (difference > 0.0f)
						{
							srcs[0].x += difference * srcFactor.y;
							positions[1].y -= difference;
						}
						v.x = positions[0].x;	v.y = positions[0].y;	v.u = srcs[1].x;	v.v = srcs[0].y;	this->tileVertices += v;
						v.x = positions[1].x;	v.y = positions[0].y;	v.u = srcs[1].x;	v.v = srcs[1].y;	this->tileVertices += v;
						v.x = positions[0].x;	v.y = positions[1].y;	v.u = srcs[0].x;	v.v = srcs[0].y;	this->tileVertices += v;
						this->tileVertices += this->tileVertices(-2, 2);
						v.x = positions[1].x;	v.y = positions[1].y;	v.u = srcs[0].x;	v.v = srcs[1].y;	this->tileVertices += v;
					}
				}
			}
		}
	}

	void TileImage::draw(grect rect, april::Color color)
	{
		if (this->color != april::Color::White)
		{
			color *= this->color;
		}
		if (color.a == 0)
		{
			return;
		}
		if (this->texture != NULL) // to prevent a crash in Texture::load so that a possible crash happens below instead
		{
			this->texture->load();
		}
		april::rendersys->setTexture(this->texture->getTexture());
		bool recreateVertices = !this->_textureCoordinatesLoaded;
		this->tryLoadTextureCoordinates();
		if (recreateVertices || this->_lastScroll != this->scroll)
		{
			this->_createVertices(rect);
			this->_lastScroll = this->scroll;
		}
		april::rendersys->setTextureBlendMode(this->blendMode);
		april::rendersys->setTextureColorMode(this->colorMode, this->colorModeFactor);
		if (this->tileVertices.size() == 0)
		{
			return;
		}
		if (color.r < 255 || color.g < 255 || color.b < 255 || color.a < 255)
		{
			april::rendersys->render(april::RO_TRIANGLE_LIST, &this->tileVertices[0], this->tileVertices.size(), color);
		}
		else
		{
			april::rendersys->render(april::RO_TRIANGLE_LIST, &this->tileVertices[0], this->tileVertices.size());
		}
		april::rendersys->setTextureBlendMode(april::BM_DEFAULT);
		april::rendersys->setTextureColorMode(april::CM_DEFAULT);
	}

}

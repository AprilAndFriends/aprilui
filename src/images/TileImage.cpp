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

#include "aprilui.h"
#include "apriluiUtil.h"
#include "Texture.h"
#include "TileImage.h"

namespace aprilui
{
	hmap<hstr, PropertyDescription> TileImage::_propertyDescriptions;
	hmap<hstr, PropertyDescription::Accessor*> TileImage::_getters;
	hmap<hstr, PropertyDescription::Accessor*> TileImage::_setters;

	TileImage::TileImage(Texture* texture, chstr name, cgrectf source) : Image(texture, name, source)
	{
		this->tile = source.getSize();
		this->useTileCount = false;
	}

	TileImage::TileImage(const TileImage& other) : Image(other)
	{
		this->tile = other.tile;
		this->scroll = other.scroll;
		this->useTileCount = other.useTileCount;
		this->tileVertices = other.tileVertices;
		this->_lastScroll = other._lastScroll;
		this->_lastRect = other._lastRect;
	}

	TileImage::~TileImage()
	{
	}

	MinimalImage* TileImage::createInstance(Texture* texture, chstr name, cgrectf source)
	{
		return new TileImage(texture, name, source);
	}

	hmap<hstr, PropertyDescription>& TileImage::getPropertyDescriptions() const
	{
		if (TileImage::_propertyDescriptions.size() == 0)
		{
			TileImage::_propertyDescriptions = Image::getPropertyDescriptions();
			TileImage::_propertyDescriptions["tile"] = PropertyDescription("tile", PropertyDescription::Type::Gvec2f);
			TileImage::_propertyDescriptions["tile_w"] = PropertyDescription("tile_w", PropertyDescription::Type::Float);
			TileImage::_propertyDescriptions["tile_h"] = PropertyDescription("tile_h", PropertyDescription::Type::Float);
			TileImage::_propertyDescriptions["scroll"] = PropertyDescription("scroll", PropertyDescription::Type::Gvec2f);
			TileImage::_propertyDescriptions["scroll_x"] = PropertyDescription("scroll_x", PropertyDescription::Type::Float);
			TileImage::_propertyDescriptions["scroll_y"] = PropertyDescription("scroll_y", PropertyDescription::Type::Float);
			TileImage::_propertyDescriptions["use_tile_count"] = PropertyDescription("use_tile_count", PropertyDescription::Type::Bool);
		}
		return TileImage::_propertyDescriptions;
	}

	hmap<hstr, PropertyDescription::Accessor*>& TileImage::_getGetters() const
	{
		if (TileImage::_getters.size() == 0)
		{
			TileImage::_getters = Image::_getGetters();
			TileImage::_getters["tile"] = new PropertyDescription::GetGvec2f<TileImage>(&TileImage::getTile);
			TileImage::_getters["tile_w"] = new PropertyDescription::Get<TileImage, float>(&TileImage::getTileWidth);
			TileImage::_getters["tile_h"] = new PropertyDescription::Get<TileImage, float>(&TileImage::getTileHeight);
			TileImage::_getters["scroll"] = new PropertyDescription::GetGvec2f<TileImage>(&TileImage::getScroll);
			TileImage::_getters["scroll_x"] = new PropertyDescription::Get<TileImage, float>(&TileImage::getScrollX);
			TileImage::_getters["scroll_y"] = new PropertyDescription::Get<TileImage, float>(&TileImage::getScrollY);
			TileImage::_getters["use_tile_count"] = new PropertyDescription::Get<TileImage, bool>(&TileImage::isUseTileCount);
		}
		return TileImage::_getters;
	}

	hmap<hstr, PropertyDescription::Accessor*>& TileImage::_getSetters() const
	{
		if (TileImage::_setters.size() == 0)
		{
			TileImage::_setters = Image::_getSetters();
			TileImage::_setters["tile"] = new PropertyDescription::SetGvec2f<TileImage>(&TileImage::setTile);
			TileImage::_setters["tile_w"] = new PropertyDescription::Set<TileImage, float>(&TileImage::setTileWidth);
			TileImage::_setters["tile_h"] = new PropertyDescription::Set<TileImage, float>(&TileImage::setTileHeight);
			TileImage::_setters["scroll"] = new PropertyDescription::SetGvec2f<TileImage>(&TileImage::setScroll);
			TileImage::_setters["scroll_x"] = new PropertyDescription::Set<TileImage, float>(&TileImage::setScrollX);
			TileImage::_setters["scroll_y"] = new PropertyDescription::Set<TileImage, float>(&TileImage::setScrollY);
			TileImage::_setters["use_tile_count"] = new PropertyDescription::Set<TileImage, bool>(&TileImage::setUseTileCount);
		}
		return TileImage::_setters;
	}

	void TileImage::_createVertices(grectf rect)
	{
		this->tileVertices.clear();
		gvec2f positions[2] = { rect.getTopLeft(), rect.getBottomRight() };
		gvec2f srcs[2];
		gvec2f scroll = this->scroll;
		gvec2f tile = this->tile;
		april::TexturedVertex v;
		april::Texture* renderTexture = this->texture->getTexture();
		gvec2f srcSize = this->srcRect.getSize();
		bool fullTexture = (this->texture->isValid() && renderTexture->getAddressMode() == april::Texture::AddressMode::Wrap &&
			this->srcRect.x == 0.0f && this->srcRect.y == 0.0f && srcSize.x == (float)this->texture->getWidth() && srcSize.y == (float)this->texture->getHeight());
		if (fullTexture)
		{
			if (!this->useTileCount)
			{
				tile = rect.getSize() / tile;
			}
			srcs[0] = -scroll / rect.getSize() * tile;
			// used to avoid going too far beyond [0, 1] since some shader-based renderers use low-precision floating point values for UV coordinates
			srcs[0].x = hmodf(srcs[0].x, 1.0f);
			srcs[0].y = hmodf(srcs[0].y, 1.0f);
			srcs[1] = srcs[0] + tile;
			v.x = positions[0].x;	v.y = positions[0].y;	v.u = srcs[0].x;	v.v = srcs[0].y;	this->tileVertices += v;
			v.x = positions[1].x;	v.y = positions[0].y;	v.u = srcs[1].x;	v.v = srcs[0].y;	this->tileVertices += v;
			v.x = positions[0].x;	v.y = positions[1].y;	v.u = srcs[0].x;	v.v = srcs[1].y;	this->tileVertices += v;
			this->tileVertices += this->tileVertices(-2, 2);
			v.x = positions[1].x;	v.y = positions[1].y;	v.u = srcs[1].x;	v.v = srcs[1].y;	this->tileVertices += v;
		}
		else
		{
			if (this->useTileCount)
			{
				tile = rect.getSize() / tile;
			}
			gvec2f invSize(1.0f / this->texture->getWidth(), 1.0f / this->texture->getHeight());
			grectf invSrcRect(this->srcRect.getPosition() * invSize, this->srcRect.getSize() * invSize);
			gvec2f srcFactor = invSrcRect.getSize() / tile;
			float difference = 0.0f;
			scroll.x = hmodf(this->scroll.x, tile.x) - tile.x;
			scroll.y = hmodf(this->scroll.y, tile.y) - tile.y;
			int countX = hceil((rect.w - scroll.x) / tile.x);
			int countY = hceil((rect.h - scroll.y) / tile.y);
			int i = 0;
			int j = 0;
			for_iterx (j, 0, countY)
			{
				for_iterx (i, 0, countX)
				{
					positions[0] = rect.getPosition() + scroll + gvec2f((float)i, (float)j) * tile;
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
	}

	void TileImage::draw(cgrectf rect, const april::Color& color)
	{
		if (color.a == 0 || this->color.a == 0)
		{
			return;
		}
		april::Color drawColor = color;
		if (this->color != april::Color::White)
		{
			drawColor *= this->color;
			if (drawColor.a == 0)
			{
				return;
			}
		}
		if (this->texture != NULL) // to prevent a crash in Texture::load so that a possible crash happens below instead
		{
			this->texture->load();
		}
		april::rendersys->setTexture(this->texture->getTexture());
		bool recreateVertices = (!this->_textureCoordinatesLoaded || !this->_clipRectCalculated);
		this->tryLoadTextureCoordinates();
		if (recreateVertices || this->_lastScroll != this->scroll || this->_lastRect != rect)
		{
			this->_createVertices(rect);
			this->_lastScroll = this->scroll;
			this->_lastRect = rect;
		}
		april::rendersys->setBlendMode(this->blendMode);
		april::rendersys->setColorMode(this->colorMode, this->colorModeFactor);
		if (this->tileVertices.size() == 0)
		{
			return;
		}
		april::rendersys->render(april::RenderOperation::TriangleList, (april::TexturedVertex*)this->tileVertices, this->tileVertices.size(), drawColor);
	}

	void TileImage::draw(const harray<april::TexturedVertex>& vertices, const april::Color& color)
	{
		hlog::warn(logTag, "TileImage::draw(harray<april::TexturedVertex>, april::Color) is not supported!");
	}

}

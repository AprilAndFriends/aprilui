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

	void TileImage::draw(grect rect, april::Color color)
	{
		if (color.a == 0)
		{
			return;
		}
		gvec2 tile = this->tile;
		if (this->useTileCount)
		{
			tile = rect.getSize() / this->tile;
		}
		gvec2 scroll;
		scroll.x = hmodf(this->scroll.x, tile.x) - tile.x;
		scroll.y = hmodf(this->scroll.y, tile.y) - tile.y;
		int countX = (int)ceil((rect.w - scroll.x) / tile.x);
		int countY = (int)ceil((rect.h - scroll.y) / tile.y);
		int i;
		int j;
		int minX = 0;
		int maxX = countX - 1;
		int minY = 0;
		int maxY = countY - 1;
		int clipped;
		april::Texture* renderTexture = this->texture->getTexture();
		bool fullTexture = (this->texture->isValid() && renderTexture->isLoaded() &&
			renderTexture->getAddressMode() == april::Texture::ADDRESS_WRAP &&
			this->srcRect == grect(0.0f, 0.0f, (float)this->texture->getWidth(), (float)this->texture->getHeight()));
		// TODO - this can be optimized further by rendering corners separately, then the edges in one call and finally the center piece in one call
		for_iterx (j, 0, countY)
		{
			for_iterx (i, 0, countX)
			{
				clipped = this->_drawTile(rect, grect(rect.getPosition() + scroll + gvec2((float)i, (float)j) * tile, tile), color, fullTexture);
				if ((clipped & 0x0010) != 0x0)
				{
					minX = i + 1;
				}
				if ((clipped & 0x0100) != 0x0)
				{
					maxX = i;
				}
				if ((clipped & 0x0001) != 0x0)
				{
					minY = j + 1;
				}
				if ((clipped & 0x1000) != 0x0)
				{
					maxY = j;
				}
			}
		}
		if (fullTexture && minX < maxX && minY < maxY)
		{
			grect src = this->srcRect;
			this->srcRect.w *= (maxX - minX);
			this->srcRect.h *= (maxY - minY);
			this->_textureCoordinatesLoaded = false; // srcRect has been changed
			Image::draw(grect(rect.getPosition() + scroll + gvec2((float)minX, (float)minY) * tile, gvec2((float)(maxX - minX), (float)(maxY - minY)) * tile), color);
			this->srcRect = src;
		}
	}

	int TileImage::_drawTile(grect rect, grect tileRect, april::Color color, bool fullTexture)
	{
		int clipped = 0;
		float difference = 0.0f;
		float srcDifference = 0.0f;
		grect src = this->srcRect;
		if (tileRect.x <= rect.x)
		{
			difference = rect.x - tileRect.x;
			srcDifference = src.w * difference / tileRect.w;
			this->srcRect.x += srcDifference;
			this->srcRect.w -= srcDifference;
			tileRect.x += difference;
			tileRect.w -= difference;
			clipped |= 0x0010;
		}
		if (tileRect.x + tileRect.w >= rect.x + rect.w)
		{
			difference = tileRect.x + tileRect.w - (rect.x + rect.w);
			srcDifference = src.w * difference / tileRect.w;
			this->srcRect.w -= srcDifference;
			tileRect.w -= difference;
			clipped |= 0x0100;
		}
		if (tileRect.y <= rect.y)
		{
			difference = rect.y - tileRect.y;
			srcDifference = src.h * difference / tileRect.h;
			this->srcRect.y += srcDifference;
			this->srcRect.h -= srcDifference;
			tileRect.y += difference;
			tileRect.h -= difference;
			clipped |= 0x0001;
		}
		if (tileRect.y + tileRect.h >= rect.y + rect.h)
		{
			difference = tileRect.y + tileRect.h - (rect.y + rect.h);
			srcDifference = src.h * difference / tileRect.h;
			this->srcRect.h -= srcDifference;
			tileRect.h -= difference;
			clipped |= 0x1000;
		}
		if (!fullTexture || clipped != 0)
		{
			this->_textureCoordinatesLoaded = false; // srcRect has been changed
			Image::draw(tileRect, color);
		}
		this->srcRect = src;
		return (fullTexture ? clipped : 0);
	}

}

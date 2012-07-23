/// @file
/// @author  Boris Mikic
/// @version 1.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <math.h>

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>

#include "Texture.h"
#include "TiledImage.h"

namespace aprilui
{
	TiledImage::TiledImage(Texture* texture, chstr name, grect source, bool vertical, float tileW, float tileH) :
		Image(texture, name, source, vertical)
	{
		this->mTile.set(tileW, tileH);
	}

	TiledImage::~TiledImage()
	{
	}

	void TiledImage::setTile(float w, float h)
	{
		this->mTile.set(w, h);
	}
	
	void TiledImage::setScroll(float x, float y)
	{
		this->mScroll.set(x, y);
	}

	void TiledImage::draw(grect rect, april::Color color)
	{
		float tileW = (this->mTile.x > 0.0f ? rect.w / this->mTile.x : -this->mTile.x);
		float tileH = (this->mTile.y > 0.0f ? rect.h / this->mTile.y : -this->mTile.y);
		float scrollX = hmodf(this->mScroll.x, tileW) - tileW;
		float scrollY = hmodf(this->mScroll.y, tileH) - tileH;
		int countX = (int)ceil((rect.w - scrollX) / tileW);
		int countY = (int)ceil((rect.h - scrollY) / tileH);
		int i;
		int j;
		for_iterx (j, 0, countY)
		{
			for_iterx (i, 0, countX)
			{
				this->_drawTile(rect, grect(rect.x + scrollX + i * tileW, rect.y + scrollY + j * tileH, tileW, tileH), color);
			}
		}
	}

	void TiledImage::_drawTile(grect rect, grect tileRect, april::Color color)
	{
		float difference;
		float srcDifference;
		grect src = this->mSrcRect;
		if (tileRect.x < rect.x)
		{
			difference = rect.x - tileRect.x;
			srcDifference = src.w * difference / tileRect.w;
			this->mSrcRect.x += srcDifference;
			this->mSrcRect.w -= srcDifference;
			tileRect.x += difference;
			tileRect.w -= difference;
		}
		if (tileRect.x + tileRect.w > rect.x + rect.w)
		{
			difference = tileRect.x + tileRect.w - (rect.x + rect.w);
			srcDifference = src.w * difference / tileRect.w;
			this->mSrcRect.w -= srcDifference;
			tileRect.w -= difference;
		}
		if (tileRect.y < rect.y)
		{
			difference = rect.y - tileRect.y;
			srcDifference = src.h * difference / tileRect.h;
			this->mSrcRect.y += srcDifference;
			this->mSrcRect.h -= srcDifference;
			tileRect.y += difference;
			tileRect.h -= difference;
		}
		if (tileRect.y + tileRect.h > rect.y + rect.h)
		{
			difference = tileRect.y + tileRect.h - (rect.y + rect.h);
			srcDifference = src.h * difference / tileRect.h;
			this->mSrcRect.h -= srcDifference;
			tileRect.h -= difference;
		}
		this->mTextureCoordinatesLoaded = false; // srcRect has been changed
		Image::draw(tileRect, color);
		this->mSrcRect = src;
	}

}

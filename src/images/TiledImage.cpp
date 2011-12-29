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

#include "TiledImage.h"

namespace aprilui
{
	TiledImage::TiledImage(april::Texture* texture, chstr name, grect source, bool vertical, float tileW, float tileH) :
		Image(texture, name, source, vertical)
	{
		mTile.set(tileW, tileH);
	}

	TiledImage::~TiledImage()
	{
	}

	void TiledImage::setTile(float w, float h)
	{
		mTile.set(w, h);
	}
	
	void TiledImage::setScroll(float x, float y)
	{
		mScroll.set(x, y);
	}

	void TiledImage::draw(grect rect, april::Color color)
	{
		float tileW = (mTile.x > 0.0f ? rect.w / mTile.x : -mTile.x);
		float tileH = (mTile.y > 0.0f ? rect.h / mTile.y : -mTile.y);
		float scrollX = fmod(mScroll.x, tileW);
		if (scrollX > 0.0f)
		{
			scrollX -= tileW;
		}
		float scrollY = fmod(mScroll.y, tileH);
		if (scrollY > 0.0f)
		{
			scrollY -= tileH;
		}
		int countX = (int)ceil((rect.w - scrollX) / tileW);
		int countY = (int)ceil((rect.h - scrollY) / tileH);
		for (int j = 0; j < countY; j++)
		{
			for (int i = 0; i < countX; i++)
			{
				_drawTile(rect, grect(rect.x + scrollX + i * tileW, rect.y + scrollY + j * tileH, tileW, tileH), color);
			}
		}
	}

	void TiledImage::_drawTile(grect rect, grect tileRect, april::Color color)
	{
		float difference;
		float srcDifference;
		grect src = mSrcRect;
		if (tileRect.x < rect.x)
		{
			difference = rect.x - tileRect.x;
			srcDifference = src.w * difference / tileRect.w;
			mSrcRect.x += srcDifference;
			mSrcRect.w -= srcDifference;
			tileRect.x += difference;
			tileRect.w -= difference;
		}
		if (tileRect.x + tileRect.w > rect.x + rect.w)
		{
			difference = tileRect.x + tileRect.w - (rect.x + rect.w);
			srcDifference = src.w * difference / tileRect.w;
			mSrcRect.w -= srcDifference;
			tileRect.w -= difference;
		}
		if (tileRect.y < rect.y)
		{
			difference = rect.y - tileRect.y;
			srcDifference = src.h * difference / tileRect.h;
			mSrcRect.y += srcDifference;
			mSrcRect.h -= srcDifference;
			tileRect.y += difference;
			tileRect.h -= difference;
		}
		if (tileRect.y + tileRect.h > rect.y + rect.h)
		{
			difference = tileRect.y + tileRect.h - (rect.y + rect.h);
			srcDifference = src.h * difference / tileRect.h;
			mSrcRect.h -= srcDifference;
			tileRect.h -= difference;
		}
		Image::draw(tileRect, color);
		mSrcRect = src;
	}

}

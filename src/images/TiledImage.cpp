/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
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
		mTileW = tileW;
		mTileH = tileH;
		mScrollX = 0.0f;
		mScrollY = 0.0f;
	}

	void TiledImage::setTile(float w, float h)
	{
		mTileW = w;
		mTileH = h;
	}
	
	void TiledImage::setScroll(float x, float y)
	{
		mScrollX = x;
		mScrollY = y;
	}

	void TiledImage::draw(grect rect, april::Color color)
	{
		float basew = (mTileW > 0 ? rect.w / mTileW : -mTileW);
		float baseh = (mTileH > 0 ? rect.h / mTileH : -mTileH);
		float ox = fabs(mScrollX);
		float oy = fabs(mScrollY);
			  
		if (mScrollX != 0)
		{
			ox = fmod(ox, basew);
			if (mScrollX < 0)
			{
				ox = basew - ox;
			}
			rect.x += ox;
			rect.w -= ox;
		}

		if (mScrollY != 0)
		{
			oy = fmod(oy, baseh);
			if (mScrollY < 0)
			{
				oy = baseh - oy;
			}
			rect.y += oy;
			rect.h -= oy;
		}
		
		float tilew = rect.w / basew;
		float tileh = rect.h / baseh;
		
		for (int j = 0; j < (int)tileh; j++)
		{
			for (int i = 0; i < (int)tilew; i++)
			{
				Image::draw(grect(rect.x + i * basew, rect.y + j * baseh, basew, baseh), color);
			}
		}
		
		float osx = mSrcRect.x;
		float osy = mSrcRect.y;
		float osw = mSrcRect.w;
		float osh = mSrcRect.h;
		// RIGHT
		if (tilew-(int) tilew > 0)
		{
			mSrcRect.w = (rect.w - (int)tilew * basew) * osw / basew;
			float dx = rect.x + (int)tilew * basew;
			_updateTexCoords();
			for (int j = 0; j < (int)tileh; j++)
			{
				Image::draw(grect(dx, rect.y + j * baseh, (rect.w - (int)tilew * basew), baseh), color);
			}
			mSrcRect.w = osw;
		}
		// LEFT
		if (ox > 0)
		{
			mSrcRect.w = ox / basew * osw;
			mSrcRect.x = osx + (basew - ox) / basew * osw;
			_updateTexCoords();
			for (int j = 0; j < (int)tileh; j++)
			{
				Image::draw(grect(rect.x - ox, rect.y + j * baseh, ox, baseh), color);
			}
			mSrcRect.x = osx;
			mSrcRect.w = osw;
		}
		// DOWN
		if (tileh - (int)tileh > 0)
		{
			mSrcRect.h = (rect.h - (int)tileh * baseh) * osh / baseh;
			float dy = rect.y + (int)tileh * baseh;
			_updateTexCoords();
			for (int i = 0; i < (int)tilew; i++)
			{
				Image::draw(grect(rect.x + i * basew, dy, basew, rect.h - (int)tileh * baseh), color);
			}
			mSrcRect.h = osh;
		}
		// UP
		if (oy > 0)
		{
			mSrcRect.h = oy / baseh * osh;
			mSrcRect.y = osy + (baseh - oy) / baseh * osh;
			_updateTexCoords();
			for (int i = 0; i < (int)tilew; i++)
			{
				Image::draw(grect(rect.x + i * basew, rect.y - oy, basew, oy), color);
			}
			mSrcRect.y = osy;
			mSrcRect.w = osw;
		}
		
		if (ox > 0 && oy > 0)
		{
			// UPPER-LEFT CORNER
			mSrcRect.w = ox / basew * osw;
			mSrcRect.h = oy / baseh * osh;
			mSrcRect.x = osx + (basew - ox) / basew * osw;
			mSrcRect.y = osy + (baseh - oy) / baseh * osh;
			_updateTexCoords();
			Image::draw(grect(rect.x - ox, rect.y - oy, ox, oy), color);
			
			// UPPER-RIGHT CORNER
			mSrcRect.w = (rect.w - (int)tilew * basew) * osw / basew;
			mSrcRect.h = oy / baseh * osh;
			mSrcRect.x = osx;
			mSrcRect.y = osy + (baseh - oy) / baseh * osh;
			_updateTexCoords();
			Image::draw(grect(rect.x + (int)tilew * basew, rect.y - oy, rect.w - (int)tilew * basew, oy), color);
			
			// LOWER-LEFT CORNER
			mSrcRect.w = ox / basew * osw;
			mSrcRect.h = (rect.h - (int)tileh * baseh) * osh / baseh;
			mSrcRect.x = osx + (basew - ox) / basew * osw;
			mSrcRect.y = osy;
			_updateTexCoords();
			Image::draw(grect(rect.x - ox, rect.y + (int)tileh * baseh, ox, rect.h - (int)tileh * baseh), color);
			
			mSrcRect.x = osx;
			mSrcRect.y = osy;
			mSrcRect.w = osw;
			mSrcRect.h = osh;
		}
		
		// LOWER-RIGHT CORNER
		if (tilew - (int)tilew > 0 && tileh - (int)tileh > 0)
		{
			mSrcRect.w = rect.w - (int)tilew * basew * osw / basew;
			mSrcRect.h = rect.h - (int)tileh * baseh * osh / baseh;
			_updateTexCoords();
			Image::draw(grect(rect.x + (int)tilew * basew, rect.y + (int)tileh * baseh,
							  rect.w - (int)tilew * basew, rect.h - (int)tileh * baseh), color);
		}
		
		if (tilew - (int)tilew > 0 || tileh - (int)tileh > 0 || ox > 0 || oy > 0)
		{
			mSrcRect.x = osx;
			mSrcRect.y = osy;
			mSrcRect.w = osw;
			mSrcRect.h = osh;
			_updateTexCoords();
		}
	}

	void TiledImage::draw(grect rect, april::Color color, float angle, gvec2 center)
	{
		//2DO
		draw(rect, color);
	}


}

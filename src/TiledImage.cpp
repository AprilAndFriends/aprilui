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

#include "TiledImage.h"

namespace AprilUI
{
	TiledImage::TiledImage(April::Texture* texture, chstr name, grect source, bool vertical, float tileW, float tileH) :
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

	void TiledImage::draw(float x, float y, float w, float h, float r, float g, float b, float a)
	{
		float basew = (mTileW > 0 ? w / mTileW : -mTileW);
		float baseh = (mTileH > 0 ? h / mTileH : -mTileH);
		float ox = fabs(mScrollX);
		float oy = fabs(mScrollY);
			  
		if (mScrollX != 0)
		{
			ox = fmod(ox, basew);
			if (mScrollX < 0)
			{
				ox = basew - ox;
			}
			x += ox;
			w -= ox;
		}

		if (mScrollY != 0)
		{
			oy = fmod(oy, baseh);
			if (mScrollY < 0)
			{
				oy = baseh - oy;
			}
			y += oy;
			h -= oy;
		}
		
		float tilew = w / basew;
		float tileh = h / baseh;
		
		for (int j = 0; j < (int)tileh; j++)
		{
			for (int i = 0; i < (int)tilew; i++)
			{
				Image::draw(x + i * basew, y + j * baseh, basew, baseh, r ,g ,b, a);
			}
		}
		
		float osx = mSource.x;
		float osy = mSource.y;
		float osw = mSource.w;
		float osh = mSource.h;
		// RIGHT
		if (tilew-(int) tilew > 0)
		{
			mSource.w = (w - (int)tilew * basew) * osw / basew;
			float dx = x + (int)tilew * basew;
			_updateTexCoords();
			for (int j = 0; j < (int)tileh; j++)
			{
				Image::draw(dx, y + j * baseh, (w - (int)tilew * basew), baseh, r, g, b, a);
			}
			mSource.w = osw;
		}
		// LEFT
		if (ox > 0)
		{
			mSource.w = ox / basew * osw;
			mSource.x = osx + (basew - ox) / basew * osw;
			_updateTexCoords();
			for (int j = 0; j < (int)tileh; j++)
			{
				Image::draw(x - ox, y + j * baseh, ox, baseh, r, g, b, a);
			}
			mSource.x = osx;
			mSource.w = osw;
		}
		// DOWN
		if (tileh - (int)tileh > 0)
		{
			mSource.h = (h - (int)tileh * baseh) * osh / baseh;
			float dy = y + (int)tileh * baseh;
			_updateTexCoords();
			for (int i = 0; i < (int)tilew; i++)
			{
				Image::draw(x + i * basew, dy, basew, (h - (int)tileh * baseh), r, g, b, a);
			}
			mSource.h = osh;
		}
		// UP
		if (oy > 0)
		{
			mSource.h = oy / baseh * osh;
			mSource.y = osy + (baseh - oy) / baseh * osh;
			_updateTexCoords();
			for (int i = 0; i < (int)tilew; i++)
			{
				Image::draw(x + i * basew, y - oy, basew, oy, r, g, b, a);
			}
			mSource.y = osy;
			mSource.w = osw;
		}
		
		if (ox > 0 && oy > 0)
		{
			// UPPER-LEFT CORNER
			mSource.w = ox / basew * osw;
			mSource.h = oy / baseh * osh;
			mSource.x = osx + (basew - ox) / basew * osw;
			mSource.y = osy + (baseh - oy) / baseh * osh;
			_updateTexCoords();
			Image::draw(x - ox, y - oy, ox, oy, r, g, b, a);
			
			// UPPER-RIGHT CORNER
			mSource.w = (w - (int)tilew * basew) * osw / basew;
			mSource.h = oy / baseh * osh;
			mSource.x = osx;
			mSource.y = osy + (baseh - oy) / baseh * osh;
			_updateTexCoords();
			Image::draw(x + (int)tilew * basew, y - oy, w - (int)tilew * basew, oy, r, g, b, a);
			
			// LOWER-LEFT CORNER
			mSource.w = ox / basew * osw;
			mSource.h = (h - (int)tileh * baseh) * osh / baseh;
			mSource.x = osx + (basew - ox) / basew * osw;
			mSource.y = osy;
			_updateTexCoords();
			Image::draw(x - ox, y + (int)tileh * baseh, ox, h - (int)tileh * baseh, r, g, b, a);
			
			mSource.x = osx;
			mSource.y = osy;
			mSource.w = osw;
			mSource.h = osh;
		}
		
		// LOWER-RIGHT CORNER
		if (tilew - (int)tilew > 0 && tileh - (int)tileh > 0)
		{
			mSource.w = w - (int)tilew * basew * osw / basew;
			mSource.h = h - (int)tileh * baseh * osh / baseh;
			_updateTexCoords();
			Image::draw(x + (int)tilew * basew, y + (int)tileh * baseh,
					    w - (int)tilew * basew, h - (int)tileh * baseh, r, g, b, a);
		}
		
		if (tilew - (int)tilew > 0 || tileh - (int)tileh > 0 || ox > 0 || oy > 0)
		{
			mSource.x = osx;
			mSource.y = osy;
			mSource.w = osw;
			mSource.h = osh;
			_updateTexCoords();
		}
	}

	void TiledImage::draw(float x, float y, float w, float h, float r, float g, float b, float a, float angle)
	{
		
	}


}

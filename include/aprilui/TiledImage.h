/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_TILED_IMAGE_H
#define APRILUI_TILED_IMAGE_H

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>

#include "Image.h"

#include "AprilUIExport.h"

namespace AprilUI
{
	class AprilUIExport TiledImage : public Image
	{
	public:
		TiledImage(April::Texture* texture, chstr name, grect source, bool vertical, float tileW, float tileH);
		
		float getTileW() { return mTileW; }
		void setTileW(float w) { mTileW = w; }
		float getTileH() { return mTileH; }
		void setTileH(float h) { mTileW = h; }
		float getScrollX() { return mScrollX; }
		void setScrollX(float x) { mScrollX = x; }
		float getScrollY() { return mScrollY; }
		void setScrollY(float y) { mScrollY = y; }
		
		void setTile(float w, float h);
		void setScroll(float x, float y);
		
		void draw(float x, float y, float w, float h, float r, float g, float b, float a);
		void draw(float x, float y, float w, float h, float r, float g, float b, float a, float angle);
		
	protected:
		float mTileW;
		float mTileH;
		float mScrollX;
		float mScrollY;
		
	};
	
}
#endif

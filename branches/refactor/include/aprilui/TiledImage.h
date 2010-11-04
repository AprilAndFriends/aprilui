/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_TILED_IMAGE_H
#define APRILUI_TILED_IMAGE_H

#include <april/RenderSystem.h>

#include "Image.h"

#include "AprilUIExport.h"

namespace AprilUI
{
	class AprilUIExport TiledImage : public Image
	{
		float mTileW,mTileH,mScrollX,mScrollY;
	public:
		TiledImage(April::Texture* tex,chstr name,float sx,float sy,float sw,float sh,bool vertical,float tilew,float tileh);
		void setTileW(float tile) { mTileW=tile; }
		void setTileH(float tile) { mTileW=tile; }
		void setTiles(float tilew,float tileh) { mTileW=tilew; mTileH=tileh; }
		float getTileW() { return mTileW; }
		float getTileH() { return mTileH; }

		void setScrollX(float scroll) { mScrollX=scroll; }
		void setScrollY(float scroll) { mScrollY=scroll; }
		void setScroll(float scrollx,float scrolly) { mScrollX=scrollx; mScrollY=scrolly; }
		float getScrollX() { return mScrollX; }
		float getScrollY() { return mScrollY; }	

		void draw(float dx,float dy,float dw,float dh,float r,float g,float b,float a);
		void draw(float centerx,float centery,float dw,float dh,float angle,float r,float g,float b,float a);
	};
	
}
#endif

/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifndef APRILUI_TILED_IMAGE_H
#define APRILUI_TILED_IMAGE_H

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>

#include "Image.h"

#include "apriluiExport.h"

namespace aprilui
{
	class apriluiExport TiledImage : public Image
	{
	public:
		TiledImage(Texture* texture, chstr name, grectf source, bool vertical, float tileW, float tileH);
		
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
		
		void draw(grectf rect, april::Color color);
		void draw(grectf rect, april::Color color, float angle, gvec2f center);
		
	protected:
		float mTileW;
		float mTileH;
		float mScrollX;
		float mScrollY;
		
	};
	
}
#endif

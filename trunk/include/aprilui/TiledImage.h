/// @file
/// @author  Boris Mikic
/// @version 2.5
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Defines a special image that can be rendered tiled.

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
		TiledImage(Texture* texture, chstr name, grect source, bool vertical, float tileW, float tileH);
		~TiledImage();
		
		gvec2 getTile() { return this->mTile; }
		void setTile(gvec2 value) { this->mTile = value; }
		float getTileW() { return this->mTile.x; }
		void setTileW(float value) { this->mTile.x = value; }
		float getTileH() { return this->mTile.y; }
		void setTileH(float value) { this->mTile.y = value; }
		gvec2 getScroll() { return this->mScroll; }
		void setScroll(gvec2 value) { this->mScroll = value; }
		float getScrollX() { return this->mScroll.x; }
		void setScrollX(float value) { this->mScroll.x = value; }
		float getScrollY() { return this->mScroll.y; }
		void setScrollY(float value) { this->mScroll.y = value; }
		
		void setTile(float w, float h);
		void setScroll(float x, float y);
		
		void draw(grect rect, april::Color color);
		
	protected:
		gvec2 mTile;
		gvec2 mScroll;

		void _drawTile(grect rect, grect tileRect, april::Color color);
		
	};
	
}
#endif

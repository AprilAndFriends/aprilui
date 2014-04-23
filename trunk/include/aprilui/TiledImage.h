/// @file
/// @author  Boris Mikic
/// @version 3.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
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
		TiledImage(Texture* texture, chstr name, grect source, bool vertical, bool invertX, bool invertY, float tileW, float tileH);
		~TiledImage();
		
		HL_DEFINE_GETSET(gvec2, tile, Tile);
		HL_DEFINE_GETSET(gvec2, scroll, Scroll);
		inline float getTileW() { return this->tile.x; }
		inline void setTileW(float value) { this->tile.x = value; }
		inline float getTileH() { return this->tile.y; }
		inline void setTileH(float value) { this->tile.y = value; }
		inline float getScrollX() { return this->scroll.x; }
		inline void setScrollX(float value) { this->scroll.x = value; }
		inline float getScrollY() { return this->scroll.y; }
		inline void setScrollY(float value) { this->scroll.y = value; }
		
		void setProperty(chstr name, chstr value);
		hstr getProperty(chstr name);

		void setTile(float w, float h);
		void setScroll(float x, float y);
		
		void draw(grect rect, april::Color color);
		
	protected:
		gvec2 tile;
		gvec2 scroll;

		int _drawTile(grect rect, grect tileRect, april::Color color, bool fullTexture);
		
	};
	
}
#endif

/// @file
/// @author  Boris Mikic
/// @version 3.14
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
		TiledImage(Texture* texture, chstr name, grect source, float tileW, float tileH);
		~TiledImage();
		
		HL_DEFINE_GETSET(gvec2, tile, Tile);
		inline void setTile(float w, float h) { this->tile.set(w, h); }
		HL_DEFINE_GETSET(float, tile.x, TileW);
		HL_DEFINE_GETSET(float, tile.y, TileH);
		HL_DEFINE_GETSET(gvec2, scroll, Scroll);
		inline void setScroll(float x, float y) { this->scroll.set(x, y); }
		HL_DEFINE_GETSET(float, scroll.x, ScrollX);
		HL_DEFINE_GETSET(float, scroll.y, ScrollY);
		
		virtual hstr getProperty(chstr name, bool* propertyExists = NULL);
		virtual bool setProperty(chstr name, chstr value);

		void draw(grect rect, april::Color color);
		
	protected:
		gvec2 tile;
		gvec2 scroll;

		int _drawTile(grect rect, grect tileRect, april::Color color, bool fullTexture);
		
	};
	
}
#endif

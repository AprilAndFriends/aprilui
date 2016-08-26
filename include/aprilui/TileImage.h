/// @file
/// @version 4.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines a special image that can be rendered in tiles.

#ifndef APRILUI_TILE_IMAGE_H
#define APRILUI_TILE_IMAGE_H

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>

#include "Image.h"

#include "apriluiExport.h"

namespace aprilui
{
	class apriluiExport TileImage : public Image
	{
		APRILUI_CLONEABLE(TileImage);
	public:
		TileImage(Texture* texture, chstr name, grect source);
		~TileImage();
		inline hstr getClassName() const { return "TileImage"; }
		
		HL_DEFINE_GETSET(gvec2, tile, Tile);
		inline void setTile(float w, float h) { this->tile.set(w, h); }
		HL_DEFINE_GETSET(float, tile.x, TileW);
		HL_DEFINE_GETSET(float, tile.y, TileH);
		HL_DEFINE_GETSET(gvec2, scroll, Scroll);
		inline void setScroll(float x, float y) { this->scroll.set(x, y); }
		HL_DEFINE_GETSET(float, scroll.x, ScrollX);
		HL_DEFINE_GETSET(float, scroll.y, ScrollY);
		HL_DEFINE_ISSET(useTileCount, UseTileCount);

		harray<PropertyDescription> getPropertyDescriptions() const;
		
		virtual hstr getProperty(chstr name);
		virtual bool setProperty(chstr name, chstr value);

		void draw(grect rect, april::Color color);
		
	protected:
		gvec2 tile;
		gvec2 scroll;
		bool useTileCount;
		harray<april::TexturedVertex> tileVertices;
		gvec2 _lastScroll;
		grect _lastRect;

		void _createVertices(grect rect);

	private:
		static harray<PropertyDescription> _propertyDescriptions;

	};

	DEPRECATED_ATTRIBUTE typedef TileImage TiledImage;
	
}
#endif

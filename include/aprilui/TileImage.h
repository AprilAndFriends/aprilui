/// @file
/// @version 5.0
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
		TileImage(Texture* texture, chstr name, cgrectf source);
		~TileImage();
		inline hstr getClassName() const { return "TileImage"; }
		
		static MinimalImage* createInstance(Texture* texture, chstr name, cgrectf source);

		hmap<hstr, PropertyDescription>& getPropertyDescriptions() const;

		HL_DEFINE_GETSET(gvec2f, tile, Tile);
		inline void setTile(float w, float h) { this->tile.set(w, h); }
		HL_DEFINE_GETSET(float, tile.x, TileWidth);
		HL_DEFINE_GETSET(float, tile.y, TileHeight);
		HL_DEFINE_GETSET(gvec2f, scroll, Scroll);
		inline void setScroll(float x, float y) { this->scroll.set(x, y); }
		HL_DEFINE_GETSET(float, scroll.x, ScrollX);
		HL_DEFINE_GETSET(float, scroll.y, ScrollY);
		HL_DEFINE_ISSET(useTileCount, UseTileCount);

		void draw(cgrectf rect, const april::Color& color = april::Color::White);
		void draw(const harray<april::TexturedVertex>& vertices, const april::Color& color = april::Color::White);

	protected:
		gvec2f tile;
		gvec2f scroll;
		bool useTileCount;
		harray<april::TexturedVertex> tileVertices;
		gvec2f _lastScroll;
		grectf _lastRect;

		hmap<hstr, PropertyDescription::Accessor*>& _getGetters() const;
		hmap<hstr, PropertyDescription::Accessor*>& _getSetters() const;

		void _createVertices(grectf rect);

	private:
		static hmap<hstr, PropertyDescription> _propertyDescriptions;
		static hmap<hstr, PropertyDescription::Accessor*> _getters;
		static hmap<hstr, PropertyDescription::Accessor*> _setters;

	};

}
#endif

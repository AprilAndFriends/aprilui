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
#include <gtypes/Vector2.h>

#include "Image.h"

#include "apriluiExport.h"

namespace aprilui
{
	class apriluiExport TiledImage : public Image
	{
	public:
		TiledImage(april::Texture* texture, chstr name, grect source, bool vertical, float tileW, float tileH);
		
		gvec2 getTile() { return mTile; }
		void setTile(gvec2 value) { mTile = value; }
		gvec2 getScroll() { return mScroll; }
		void setScroll(gvec2 value) { mScroll = value; }
		
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

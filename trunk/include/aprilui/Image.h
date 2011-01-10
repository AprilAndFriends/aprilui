/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_IMAGE_H
#define APRILUI_IMAGE_H

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"

namespace aprilui
{
	extern april::TexturedVertex tVertices[4];
	extern april::PlainVertex pVertices[4];
	
	class apriluiExport Image
	{
	public:
		Image(april::Texture* texture, chstr name, grect source, bool vertical = false, bool invertX = false, bool invertY = false);
		virtual ~Image();
		
		virtual void draw(grect rect);
		virtual void draw(grect rect, april::Color color);
		
		bool isVertical() { return mVertical; };
		bool isXInverted() { return mInvertX; };
		bool isYInverted() { return mInvertY; };

		april::Texture* getTexture();
		hstr getName() const { return mName; }
		hstr getImageName() const { return mImageName; }
		grect getSrcRect() const { return mSrcRect; }
		
		april::BlendMode getBlendMode() { return mBlendMode; }
		void setBlendMode(april::BlendMode mode) { mBlendMode = mode; }
		
	protected:
		april::Texture* mTexture;
		hstr mName;
		hstr mImageName;
		grect mSrcRect;
		april::BlendMode mBlendMode;
		bool mVertical;
		bool mUnloadedFlag;
		bool mInvertX;
		bool mInvertY;
		
		void _updateTexCoords();
		
	};

}
#endif

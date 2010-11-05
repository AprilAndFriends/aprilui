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
#include <hltypes/hstring.h>

#include "AprilUIExport.h"

namespace AprilUI
{
	extern April::TexturedVertex tVertices[4];
	extern April::PlainVertex pVertices[4];
	
	class AprilUIExport Image
	{
	public:
		Image(April::Texture* texture, chstr name, grect source, bool vertical = false, bool invertX = false, bool invertY = false);
		virtual ~Image();
		
		virtual void draw(grect rect);
		virtual void draw(grect rect, April::Color color);
		virtual void draw(grect rect, April::Color color, float angle);
		virtual void draw(grect rect, float angle);
		
		bool isVertical() { return mVertical; };
		bool isXInverted() { return mInvertX; };
		bool isYInverted() { return mInvertY; };

		April::Texture* getTexture();
		hstr getName() const { return mName; }
		hstr getImageName() const { return mImageName; }
		float getSourceX() const DEPRECATED_ATTRIBUTE { return mSource.x; }
		float getSourceY() const DEPRECATED_ATTRIBUTE { return mSource.y; }
		float getSourceW() const DEPRECATED_ATTRIBUTE { return mSource.w; }
		float getSourceH() const DEPRECATED_ATTRIBUTE { return mSource.h; }
		const grect& getSource() const { return mSource; }
		
		April::BlendMode getBlendMode() { return mBlendMode; }
		void setBlendMode(April::BlendMode mode) { mBlendMode = mode; }
		
	protected:
		April::Texture* mTexture;
		hstr mName;
		hstr mImageName;
		grect mSource;
		April::BlendMode mBlendMode;
		bool mVertical;
		bool mUnloadedFlag;
		bool mInvertX;
		bool mInvertY;
		
		void _updateTexCoords();
		
	};

}
#endif

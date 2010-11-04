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
#include <hltypes/hstring.h>

#include "AprilUIExport.h"

namespace AprilUI
{
	extern April::TexturedVertex tVertices[4];
	extern April::PlainVertex pVertices[4];
	
	class AprilUIExport Image
	{
	public:
		
		Image(April::Texture* texture, chstr name, float sx, float sy, float sw, float sh, bool vertical = false, bool invertX = false, bool invertY = false);
		virtual ~Image();
		
		virtual void draw(float x, float y, float w, float h, float r, float g, float b, float a);
		virtual void draw(float x, float y, float w, float h, float angle, float r, float g, float b, float a);
		virtual void draw(float x, float y, float w, float h, float angle);
		
		bool isVertical() { return mVertical; };
		bool isXInverted() { return mInvertX; };
		bool isYInverted() { return mInvertY; };

		April::Texture* getTexture();
		hstr getName() const { return mName; }
		hstr getImageName() const { return mImageName; }
		float getSourceX() const { return mSourceX; }
		float getSourceY() const { return mSourceY; }
		float getSourceW() const { return mSourceW; }
		float getSourceH() const { return mSourceH; }
		
		April::BlendMode getBlendMode() { return mBlendMode; }
		void setBlendMode(April::BlendMode mode) { mBlendMode = mode; }
		
	protected:
		April::Texture* mTexture;
		hstr mName;
		hstr mImageName;
		float mSourceX;
		float mSourceY;
		float mSourceW;
		float mSourceH;
		April::BlendMode mBlendMode;
		bool mVertical;
		bool mUnloadedFlag;
		bool mInvertX;
		bool mInvertY;
		
		void _updateTexCoords();
		
	};

}
#endif

/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
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
	class AprilUIExport Image
	{
	protected:
		April::TexturedVertex mVertices[4];
		April::Texture* mTexture;
		hstr mName;
		hstr mImageName;
		float mSourceX, mSourceY, mSourceW, mSourceH;
		April::BlendMode mBlendMode;
		bool mVertical,mUnloadedFlag,mInvertX,mInvertY;
		
		void updateTexCoords();
	public:
		
		Image(April::Texture* tex,chstr name,float sx,float sy,float sw,float sh,bool vertical=0,bool invertx=0,bool inverty=0);
		virtual ~Image();
		virtual void draw(float dx,float dy,float dw,float dh,float r,float g,float b,float a);
		virtual void draw(float centerx,float centery,float dw,float dh,float angle);
		virtual void draw(float centerx,float centery,float dw,float dh,float angle,float r,float g,float b,float a);
		
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
		void setBlendMode(April::BlendMode mode) { mBlendMode=mode; }
	};

}
#endif

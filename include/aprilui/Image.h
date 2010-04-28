/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com)                                  *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef IMAGE_H
#define IMAGE_H
#include "april/RenderSystem.h"
#include "AprilUIExport.h"

#include "hltypes/harray.h"
#include "hltypes/hstring.h"

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
		virtual void draw(float dx,float dy,float dw=-1,float dh=-1,float r=1,float g=1,float b=1,float a=1);
		virtual void draw(float centerx,float centery,float dw,float dh,float angle);
		void draw(float centerx,float centery,float dw,float dh,float angle,float r,float g,float b,float a);
		
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

	class AprilUIExport ColoredImage : public Image
	{
		float mRed,mGreen,mBlue,mAlpha;
	public:
		ColoredImage(April::Texture* tex,chstr name,float sx,float sy,float sw,float sh,bool vertical=false,unsigned int color=0xFFFFFF);
		void draw(float dx,float dy,float dw=-1,float dh=-1,float r=1,float g=1,float b=1,float a=1);
		void draw(float centerx,float centery,float dw,float dh,float angle,float r,float g,float b,float a);

		float getAlpha() { return mAlpha; }
		void setAlpha(float alpha) { mAlpha=alpha; }
		void setColor(float a,float r,float g,float b);
	};

	class AprilUIExport TiledImage : public Image
	{
		float mTileW,mTileH,mScrollX,mScrollY;
	public:
		TiledImage(April::Texture* tex,chstr name,float sx,float sy,float sw,float sh,bool vertical,float tilew,float tileh);
		void setTileW(float tile) { mTileW=tile; }
		void setTileH(float tile) { mTileW=tile; }
		void setTiles(float tilew,float tileh) { mTileW=tilew; mTileH=tileh; }
		float getTileW() { return mTileW; }
		float getTileH() { return mTileH; }

		void setScrollX(float scroll) { mScrollX=scroll; }
		void setScrollY(float scroll) { mScrollY=scroll; }
		void setScroll(float scrollx,float scrolly) { mScrollX=scrollx; mScrollY=scrolly; }
		float getScrollX() { return mScrollX; }
		float getScrollY() { return mScrollY; }	

		void draw(float dx,float dy,float dw,float dh,float r=1,float g=1,float b=1,float a=1);
		void draw(float centerx,float centery,float dw,float dh,float angle,float r,float g,float b,float a);
	};
	

	class AprilUIExport CompositeImage : public Image
	{
	public:
		struct ImageRef
		{
			Image* img;
			float x,y,w,h;
		};
	protected:
		harray<ImageRef> mImages;
	public:
		CompositeImage(chstr name,float sw,float sh);
		CompositeImage(chstr name,CompositeImage& base);
		
		void addImageRef(Image* img,float x,float y,float w,float h);
		
		void draw(float dx,float dy,float dw,float dh,float r=1,float g=1,float b=1,float a=1);
		void draw(float centerx,float centery,float dw,float dh,float angle,float r,float g,float b,float a);
		
		const harray<ImageRef>& getImageList() { return mImages; }
	};
	
	class AprilUIExport ColorImage : public Image
	{
		float mRed,mGreen,mBlue,mAlpha;
	public:
		ColorImage(chstr name);
		void draw(float dx,float dy,float dw,float dh,float r=1,float g=1,float b=1,float a=1);
		void draw(float centerx,float centery,float dw,float dh,float angle,float r,float g,float b,float a);
	};

	class AprilUIExport NullImage : public Image
	{
	public:
		NullImage();
		void draw(float dx,float dy,float dw,float dh,float r=1,float g=1,float b=1,float a=1) {}
		void draw(float centerx,float centery,float dw,float dh,float angle,float r,float g,float b,float a) {}
	};

}
#endif

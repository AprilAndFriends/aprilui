/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <math.h>

#include <april/RenderSystem.h>

#include "TiledImage.h"

namespace AprilUI
{
	TiledImage::TiledImage(April::Texture* texture, chstr name, float sx, float sy, float sw, float sh, bool vertical, float tileW, float tileH) :
		Image(texture, name, sx, sy, sw, sh, vertical)
	{
		mTileW = tileW;
		mTileH = tileH;
		mScrollX = 0.0f;
		mScrollY = 0.0f;
	}

	void TiledImage::setTile(float w, float h)
	{
		mTileW = w;
		mTileH = h;
	}
	
	void TiledImage::setScroll(float x, float y)
	{
		mScrollX = x;
		mScrollY = y;
	}

	void TiledImage::draw(float x, float y, float w, float h, float r, float g, float b, float a)
	{
		float basew=(mTileW > 0) ? w/mTileW : -mTileW,
		      baseh=(mTileH > 0) ? h/mTileH : -mTileH,
			  ox=fabs(mScrollX),oy=fabs(mScrollY);
			  
		if (mScrollX != 0)
		{
			while (ox >= basew) ox-=basew;
			if (mScrollX < 0) ox=basew-ox;
			x+=ox; w-=ox;
		}

		if (mScrollY != 0)
		{
			while (oy >= baseh) oy-=baseh;
			if (mScrollY < 0) oy=baseh-oy;
			y+=oy; h-=oy;
		}
		
		float tilew=w/basew;
		float tileh=h/baseh;
		
		for (int j=0;j<(int) tileh;j++)
			for (int i=0;i<(int) tilew;i++)
				Image::draw(x+i*basew,y+j*baseh,basew,baseh,r,g,b,a);
		
		float osx=mSourceX,osy=mSourceY,osw=mSourceW,osh=mSourceH;
		// RIGHT
		if (tilew-(int) tilew > 0)
		{
			mSourceW=(w-((int) tilew)*basew)*osw/basew;
			float dx=x+((int) tilew)*basew;
			updateTexCoords();
			for (int j=0;j<(int) tileh;j++)
				Image::draw(dx,y+j*baseh,(w-((int) tilew)*basew),baseh,r,g,b,a);
			mSourceW=osw;
		}
		// LEFT
		if (ox > 0)
		{
			mSourceW=(ox/basew)*osw;
			mSourceX=osx+((basew-ox)/basew)*osw;
			
			updateTexCoords();
			for (int j=0;j<(int) tileh;j++)
				Image::draw(x-ox,y+j*baseh,ox,baseh,r,g,b,a);
			mSourceX=osx; mSourceW=osw;
		}
		// DOWN
		if (tileh-(int) tileh > 0)
		{
			
			mSourceH=(h-((int) tileh)*baseh)*osh/baseh;
			float dy=y+((int) tileh)*baseh;
			updateTexCoords();
			for (int i=0;i<(int) tilew;i++)
				Image::draw(x+i*basew,dy,basew,(h-((int) tileh)*baseh),r,g,b,a);
			mSourceH=osh;
		}
		// UP
		if (oy > 0)
		{
			mSourceH=(oy/baseh)*osh;
			mSourceY=osy+((baseh-oy)/baseh)*osh;
			
			updateTexCoords();
			for (int i=0;i<(int) tilew;i++)
				Image::draw(x+i*basew,y-oy,basew,oy,r,g,b,a);
			mSourceY=osy; mSourceW=osw;
		}
		
		
		if (ox > 0 && oy > 0)
		{
			// UPPER-LEFT CORNER
			mSourceW=(ox/basew)*osw;
			mSourceH=(oy/baseh)*osh;
			mSourceX=osx+((basew-ox)/basew)*osw;
			mSourceY=osy+((baseh-oy)/baseh)*osh;
			updateTexCoords();
			Image::draw(x-ox,y-oy,ox,oy,r,g,b,a);
			
			// UPPER-RIGHT CORNER
			mSourceW=(w-((int) tilew)*basew)*osw/basew;
			mSourceH=(oy/baseh)*osh;
			mSourceX=osx;
			mSourceY=osy+((baseh-oy)/baseh)*osh;
			updateTexCoords();
			Image::draw(x+((int) tilew)*basew,y-oy,w-((int) tilew)*basew,oy,r,g,b,a);
			
			// LOWER-LEFT CORNER
			mSourceW=(ox/basew)*osw;
			mSourceH=(h-((int) tileh)*baseh)*osh/baseh;
			mSourceX=osx+((basew-ox)/basew)*osw;
			mSourceY=osy;
			updateTexCoords();
			Image::draw(x-ox,y+((int) tileh)*baseh,ox,h-(((int) tileh)*baseh),r,g,b,a);
			
			
			mSourceX=osx; mSourceW=osw; mSourceY=osy; mSourceW=osw;
		}
		
		// LOWER-RIGHT CORNER
		if (tilew-(int) tilew > 0 && tileh-(int) tileh > 0)
		{
			mSourceW=(w-((int) tilew)*basew)*osw/basew;
			mSourceH=(h-((int) tileh)*baseh)*osh/baseh;
			updateTexCoords();
			Image::draw(x+((int) tilew)*basew,y+((int) tileh)*baseh,
					   (w-((int) tilew)*basew),(h-((int) tileh)*baseh),r,g,b,a);
		}
		
		if (tilew-(int) tilew > 0 || tileh-(int) tileh > 0 || ox > 0 || oy > 0)
		{
			mSourceX=osx; mSourceY=osy; mSourceW=osw; mSourceH=osh;
			updateTexCoords();
		}
	}

	void TiledImage::draw(float x, float y, float w, float h, float angle, float r, float g, float b, float a)
	{
		
	}


}

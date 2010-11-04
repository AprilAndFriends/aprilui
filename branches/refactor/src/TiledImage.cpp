/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <math.h>

#include <april/RenderSystem.h>

#include "TiledImage.h"

namespace AprilUI
{
	TiledImage::TiledImage(April::Texture* tex,chstr name,float sx,float sy,float sw,float sh,bool vertical,float tilew,float tileh) :
				  Image(tex,name,sx,sy,sw,sh,vertical)
	{
		mTileW=tilew; mTileH=tileh;
		mScrollX=mScrollY=0;
	}

	void TiledImage::draw(float dx,float dy,float dw,float dh,float r,float g,float b,float a)
	{
		float basew=(mTileW > 0) ? dw/mTileW : -mTileW,
		      baseh=(mTileH > 0) ? dh/mTileH : -mTileH,
			  ox=fabs(mScrollX),oy=fabs(mScrollY);
			  
		if (mScrollX != 0)
		{
			while (ox >= basew) ox-=basew;
			if (mScrollX < 0) ox=basew-ox;
			dx+=ox; dw-=ox;
		}

		if (mScrollY != 0)
		{
			while (oy >= baseh) oy-=baseh;
			if (mScrollY < 0) oy=baseh-oy;
			dy+=oy; dh-=oy;
		}
		
		float tilew=dw/basew,tileh=dh/baseh;
		
		for (int y=0;y<(int) tileh;y++)
			for (int x=0;x<(int) tilew;x++)
				Image::draw(dx+x*basew,dy+y*baseh,basew,baseh,r,g,b,a);
		
		float osx=mSourceX,osy=mSourceY,osw=mSourceW,osh=mSourceH;
		// RIGHT
		if (tilew-(int) tilew > 0)
		{
			mSourceW=(dw-((int) tilew)*basew)*osw/basew;
			float x=dx+((int) tilew)*basew;
			updateTexCoords();
			for (int y=0;y<(int) tileh;y++)
				Image::draw(x,dy+y*baseh,(dw-((int) tilew)*basew),baseh,r,g,b,a);
			mSourceW=osw;
		}
		// LEFT
		if (ox > 0)
		{
			mSourceW=(ox/basew)*osw;
			mSourceX=osx+((basew-ox)/basew)*osw;
			
			updateTexCoords();
			for (int y=0;y<(int) tileh;y++)
				Image::draw(dx-ox,dy+y*baseh,ox,baseh,r,g,b,a);
			mSourceX=osx; mSourceW=osw;
		}
		// DOWN
		if (tileh-(int) tileh > 0)
		{
			
			mSourceH=(dh-((int) tileh)*baseh)*osh/baseh;
			float y=dy+((int) tileh)*baseh;
			updateTexCoords();
			for (int x=0;x<(int) tilew;x++)
				Image::draw(dx+x*basew,y,basew,(dh-((int) tileh)*baseh),r,g,b,a);
			mSourceH=osh;
		}
		// UP
		if (oy > 0)
		{
			mSourceH=(oy/baseh)*osh;
			mSourceY=osy+((baseh-oy)/baseh)*osh;
			
			updateTexCoords();
			for (int x=0;x<(int) tilew;x++)
				Image::draw(dx+x*basew,dy-oy,basew,oy,r,g,b,a);
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
			Image::draw(dx-ox,dy-oy,ox,oy,r,g,b,a);
			
			// UPPER-RIGHT CORNER
			mSourceW=(dw-((int) tilew)*basew)*osw/basew;
			mSourceH=(oy/baseh)*osh;
			mSourceX=osx;
			mSourceY=osy+((baseh-oy)/baseh)*osh;
			updateTexCoords();
			Image::draw(dx+((int) tilew)*basew,dy-oy,dw-((int) tilew)*basew,oy,r,g,b,a);
			
			// LOWER-LEFT CORNER
			mSourceW=(ox/basew)*osw;
			mSourceH=(dh-((int) tileh)*baseh)*osh/baseh;
			mSourceX=osx+((basew-ox)/basew)*osw;
			mSourceY=osy;
			updateTexCoords();
			Image::draw(dx-ox,dy+((int) tileh)*baseh,ox,dh-(((int) tileh)*baseh),r,g,b,a);
			
			
			mSourceX=osx; mSourceW=osw; mSourceY=osy; mSourceW=osw;
		}
		
		// LOWER-RIGHT CORNER
		if (tilew-(int) tilew > 0 && tileh-(int) tileh > 0)
		{
			mSourceW=(dw-((int) tilew)*basew)*osw/basew;
			mSourceH=(dh-((int) tileh)*baseh)*osh/baseh;
			updateTexCoords();
			Image::draw(dx+((int) tilew)*basew,dy+((int) tileh)*baseh,
					   (dw-((int) tilew)*basew),(dh-((int) tileh)*baseh),r,g,b,a);
		}
		
		if (tilew-(int) tilew > 0 || tileh-(int) tileh > 0 || ox > 0 || oy > 0)
		{
			mSourceX=osx; mSourceY=osy; mSourceW=osw; mSourceH=osh;
			updateTexCoords();
		}
	}

	void TiledImage::draw(float centerx,float centery,float dw,float dh,float angle,float r,float g,float b,float a)
	{
		
	}


}

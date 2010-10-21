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
#include <hltypes/util.h>

#include "Exception.h"
#include "Image.h"
#include "Util.h"

namespace AprilUI
{
	Image::Image(April::Texture* tex,chstr name,float sx,float sy,float sw,float sh,bool vertical,bool invertx,bool inverty)
	{
		mTexture=tex;
		mName=name;
		mImageName=name(name.find("/")+1,name.size()); // the name without the dataset's name prefix
		mSourceX=sx;
		mSourceY=sy;
		mSourceW=sw;
		mSourceH=sh;

		mBlendMode=April::ALPHA_BLEND;
		mVertical=vertical;
		mUnloadedFlag=0;
		mInvertX=invertx;
		mInvertY=inverty;

		updateTexCoords();
		mVertices[0].z=mVertices[1].z=mVertices[2].z=mVertices[3].z=0;
	}
	
	Image::~Image()
	{
		
	}
	
	void Image::updateTexCoords()
	{
		if (mTexture) // mTexture is allowed to be null, since we have derived classes
		{
 			if (!mTexture->isLoaded()) { mUnloadedFlag=1; return; }
			int w=mTexture->getWidth(),h=mTexture->getHeight();
			float t;
			if (mVertical)
			{
				mVertices[0].u=(mSourceX+mSourceH)/w; mVertices[0].v=mSourceY/h;
				mVertices[1].u=(mSourceX+mSourceH)/w; mVertices[1].v=(mSourceY+mSourceW)/h;
				mVertices[2].u=(mSourceX)/w;          mVertices[2].v=mSourceY/h;
				mVertices[3].u=(mSourceX)/w;          mVertices[3].v=(mSourceY+mSourceW)/h;
				if (mInvertY)
				{
					t=mVertices[0].u; mVertices[0].u=mVertices[2].u; mVertices[2].u=t;
					t=mVertices[1].u; mVertices[1].u=mVertices[3].u; mVertices[3].u=t;
				}
				if (mInvertX)
				{
					t=mVertices[0].v; mVertices[0].v=mVertices[1].v; mVertices[1].v=t;
					t=mVertices[2].v; mVertices[2].v=mVertices[3].v; mVertices[3].v=t;
				}
			}
			else
			{
				mVertices[0].u=mSourceX/w;            mVertices[0].v=mSourceY/h;
				mVertices[1].u=(mSourceX+mSourceW)/w; mVertices[1].v=mSourceY/h;
				mVertices[2].u=mSourceX/w;            mVertices[2].v=(mSourceY+mSourceH)/h;
				mVertices[3].u=(mSourceX+mSourceW)/w; mVertices[3].v=(mSourceY+mSourceH)/h;
				if (mInvertX)
				{
					t=mVertices[0].u; mVertices[0].u=mVertices[1].u; mVertices[1].u=t;
					t=mVertices[2].u; mVertices[2].u=mVertices[3].u; mVertices[3].u=t;
				}
				if (mInvertY)
				{
					t=mVertices[0].v; mVertices[0].v=mVertices[2].v; mVertices[2].v=t;
					t=mVertices[1].v; mVertices[1].v=mVertices[3].v; mVertices[3].v=t;
				}
			}

		}
	}

	void Image::draw(float dx,float dy,float dw,float dh,float r,float g,float b,float a)
	{
		if (dw == -1) dw=mSourceW;
		if (dh == -1) dh=mSourceH;

		mVertices[0].x=dx;    mVertices[0].y=dy;
		mVertices[1].x=dx+dw; mVertices[1].y=dy;
		mVertices[2].x=dx;    mVertices[2].y=dy+dh;
		mVertices[3].x=dx+dw; mVertices[3].y=dy+dh;
		
		
		April::rendersys->setTexture(mTexture);
		if (mUnloadedFlag && mTexture->isLoaded())
		{
			updateTexCoords();
			mUnloadedFlag=0;
		}
		if (mBlendMode != April::ALPHA_BLEND) April::rendersys->setBlendMode(mBlendMode);
		if (r != 1 || g != 1 || b != 1 || a != 1)
			April::rendersys->render(April::TriangleStrip,mVertices,4,r,g,b,a);
		else
			April::rendersys->render(April::TriangleStrip,mVertices,4);
		if (mBlendMode != April::ALPHA_BLEND) April::rendersys->setBlendMode(April::DEFAULT);
	}

	void Image::draw(float centerx,float centery,float dw,float dh,float angle,float r,float g,float b,float a)
	{
		if (dw == -1) dw=mSourceW;
		if (dh == -1) dh=mSourceH;

		mVertices[0].x=-dw/2; mVertices[0].y=-dh/2;
		mVertices[1].x= dw/2; mVertices[1].y=-dh/2;
		mVertices[2].x=-dw/2; mVertices[2].y= dh/2;
		mVertices[3].x= dw/2; mVertices[3].y= dh/2;
		
		gtypes::Matrix4 temp_matrix=April::rendersys->getModelviewMatrix();
		April::rendersys->setIdentityTransform();
		April::rendersys->translate(centerx,centery);
		April::rendersys->rotate(angle);
		April::rendersys->setTexture(mTexture);
		if (mUnloadedFlag && mTexture->isLoaded())
		{
			updateTexCoords();
			mUnloadedFlag=0;
		}
		
		if (mBlendMode != April::ALPHA_BLEND) April::rendersys->setBlendMode(mBlendMode);
		if (r != 1 || g != 1 || b != 1 || a != 1)
			April::rendersys->render(April::TriangleStrip,mVertices,4,r,g,b,a);
		else
			April::rendersys->render(April::TriangleStrip,mVertices,4);
		if (mBlendMode != April::ALPHA_BLEND) April::rendersys->setBlendMode(April::DEFAULT);
		April::rendersys->setModelviewMatrix(temp_matrix);
	}

	void Image::draw(float centerx,float centery,float dw,float dh,float angle)
	{
		draw(centerx,centerx,dw,dh,angle,1,1,1,1);
	}

	April::Texture* Image::getTexture()
	{
		return mTexture;
	}
	
	ColoredImage::ColoredImage(April::Texture* tex,chstr name,float sx,float sy,float sw,float sh,bool vertical,unsigned int color) :
				  Image(tex,name,sx,sy,sw,sh,vertical)
	{
		if (color > 0xFFFFFF)
			mAlpha=(color >> 24) / 255.0f;
		else
			mAlpha=1;
		mRed=((color >> 16) & 0xFF) / 255.0f;
		mBlue=(color & 0xFF) / 255.0f;
		mGreen=((color >> 8) & 0xFF) / 255.0f;
	}

	void ColoredImage::draw(float dx,float dy,float dw,float dh,float r,float g,float b,float a)
	{
		Image::draw(dx,dy,dw,dh,r*mRed,g*mGreen,b*mBlue,a*mAlpha);
	}

	void ColoredImage::draw(float centerx,float centery,float dw,float dh,float angle,float r,float g,float b,float a)
	{
		Image::draw(centerx,centery,dw,dh,angle,r*mRed,g*mGreen,b*mBlue,a*mAlpha);
	}

	void ColoredImage::setColor(float a,float r,float g,float b)
	{
		mAlpha=a; mRed=r; mGreen=g; mBlue=b;
	}


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


	CompositeImage::CompositeImage(chstr name,float sw,float sh) : Image(0,name,0,0,sw,sh)
	{
		
	}
	
	CompositeImage::CompositeImage(chstr name,CompositeImage& base) : Image(0,name,0,0,base.getSourceW(),base.getSourceH())
	{
		foreach(ImageRef,it,base.mImages)
			addImageRef((*it).img,(*it).x,(*it).y,(*it).w,(*it).h);
	}
	
	void CompositeImage::addImageRef(Image* img,float x,float y,float w,float h)
	{
		ImageRef ref;
		ref.img=img;
		ref.x=x; ref.y=y; ref.w=w; ref.h=h;
		
		mImages.push_back(ref);
	}

	
	void CompositeImage::draw(float dx,float dy,float dw,float dh,float r,float g,float b,float a)
	{
		float xf=dw/mSourceW,yf=dh/mSourceH;
		foreach(ImageRef,it,mImages)
		{
			(*it).img->draw(dx+(*it).x*xf,dy+(*it).y*yf,(*it).w*xf,(*it).h*yf,r,g,b,a);
		}
	}
	
	void CompositeImage::draw(float centerx,float centery,float dw,float dh,float angle,float r,float g,float b,float a)
	{
		float xf=dw/mSourceW,yf=dh/mSourceH;
		foreach(ImageRef,it,mImages)
		{
			(*it).img->draw(centerx+(*it).x*xf,centery+(*it).y*yf,(*it).w*xf,(*it).h*yf,angle,r,g,b,a);
		}
	}

	ColorImage::ColorImage(chstr name) : Image(0,name,0,0,0,0,0)
	{
		unsigned char a,r,g,b;
		hexstr_to_argb(name,&a,&r,&g,&b);
		mRed=r/255.0f;
		mGreen=g/255.0f;
		mBlue=b/255.0f;
		mAlpha=a/255.0f;
	}

	void ColorImage::draw(float dx,float dy,float dw,float dh,float r,float g,float b,float a)
	{
		April::PlainVertex v[4];
		v[0].x=dx;    v[0].y=dy;
		v[1].x=dx+dw; v[1].y=dy;
		v[2].x=dx;    v[2].y=dy+dh;
		v[3].x=dx+dw; v[3].y=dy+dh;
		
		if (mBlendMode != April::ALPHA_BLEND) April::rendersys->setBlendMode(mBlendMode);
		April::rendersys->render(April::TriangleStrip,v,4,mRed*r,mGreen*g,mBlue*b,mAlpha*a);
		if (mBlendMode != April::ALPHA_BLEND) April::rendersys->setBlendMode(April::DEFAULT);
	}

	void ColorImage::draw(float centerx,float centery,float dw,float dh,float angle,float r,float g,float b,float a)
	{
		April::PlainVertex v[4];
		v[0].x=-dw/2; v[0].y=-dh/2;
		v[1].x= dw/2; v[1].y=-dh/2;
		v[2].x=-dw/2; v[2].y= dh/2;
		v[3].x= dw/2; v[3].y= dh/2;

		
		gtypes::Matrix4 temp_matrix=April::rendersys->getModelviewMatrix();
		April::rendersys->setIdentityTransform();
		April::rendersys->translate(centerx,centery);
		April::rendersys->rotate(angle);
		if (mBlendMode != April::ALPHA_BLEND) April::rendersys->setBlendMode(mBlendMode);
		April::rendersys->render(April::TriangleStrip,v,4,mRed*r,mGreen*g,mBlue*b,mAlpha*a);
		if (mBlendMode != April::ALPHA_BLEND) April::rendersys->setBlendMode(April::DEFAULT);
		April::rendersys->setModelviewMatrix(temp_matrix);
	}

	NullImage::NullImage() : Image(0,"null",0,0,0,0,0)
	{
		
	}
}

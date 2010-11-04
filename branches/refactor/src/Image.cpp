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
	
}

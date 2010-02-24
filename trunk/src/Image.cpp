/************************************************************************************
This source file is part of the APRIL User Interface Library
For latest info, see http://libatres.sourceforge.net/
*************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License (LGPL) as published by the
Free Software Foundation; either version 2 of the License, or (at your option)
any later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*************************************************************************************/
#include "april/RenderSystem.h"
#include "Image.h"
#include "Exception.h"
#include "Util.h"

namespace AprilUI
{
	Image::Image(April::Texture* tex,std::string name,float sx,float sy,float sw,float sh,bool vertical)
	{
		mTexture=tex;
		mName=name;
		mImageName=name.substr(name.find("/")+1,name.size()); // the name without the dataset's name prefix
		mSourceX=sx;
		mSourceY=sy;
		mSourceW=sw;
		mSourceH=sh;
		mBlendMode=April::ALPHA_BLEND;
		if (tex) // allowed due to derived classes
		{
			int w=tex->getWidth(),h=tex->getHeight();
			
			if (vertical)
			{
				mVertices[0].u=sx/w;      mVertices[0].v=sy/h;
				mVertices[1].u=(sx)/w;    mVertices[1].v=(sy+sw)/h;
				mVertices[2].u=(sx-sh)/w; mVertices[2].v=(sy+sw)/h;
				mVertices[3].u=(sx-sh)/w; mVertices[3].v=sy/h;
			}
			else
			{
				mVertices[0].u=sx/w;      mVertices[0].v=sy/h;
				mVertices[1].u=(sx+sw)/w; mVertices[1].v=sy/h;
				mVertices[2].u=(sx+sw)/w; mVertices[2].v=(sy+sh)/h;
				mVertices[3].u=sx/w;      mVertices[3].v=(sy+sh)/h;
			}
			mVertices[0].z=mVertices[1].z=mVertices[2].z=mVertices[3].z=0;
		}
	}

	void Image::draw(float dx,float dy,float dw,float dh,float r,float g,float b,float a)
	{
		if (dw == -1) dw=mSourceW;
		if (dh == -1) dh=mSourceH;

		mVertices[0].x=dx;    mVertices[0].y=dy;
		mVertices[1].x=dx+dw; mVertices[1].y=dy;
		mVertices[2].x=dx+dw; mVertices[2].y=dy+dh;
		mVertices[3].x=dx;    mVertices[3].y=dy+dh;
		
		rendersys->setTexture(mTexture);
		if (mBlendMode != April::ALPHA_BLEND) rendersys->setBlendMode(mBlendMode);
		if (r != 1 || g != 1 || b != 1 || a != 1)
			rendersys->render(April::TriangleFan,mVertices,4,r,g,b,a);
		else
			rendersys->render(April::TriangleFan,mVertices,4);
		if (mBlendMode != April::ALPHA_BLEND) rendersys->setBlendMode(April::DEFAULT);
	}

	void Image::draw(float centerx,float centery,float dw,float dh,float angle,float r,float g,float b,float a)
	{

		mVertices[0].x=-dw/2; mVertices[0].y=-dh/2;
		mVertices[1].x= dw/2; mVertices[1].y=-dh/2;
		mVertices[2].x= dw/2; mVertices[2].y= dh/2;
		mVertices[3].x=-dw/2; mVertices[3].y= dh/2;
		
		rendersys->pushTransform();
		rendersys->setIdentityTransform();
		rendersys->translate(centerx,centery);
		rendersys->rotate(angle);
		rendersys->setTexture(mTexture);
		if (mBlendMode != April::ALPHA_BLEND) rendersys->setBlendMode(mBlendMode);
		if (r != 1 || g != 1 || b != 1 || a != 1)
			rendersys->render(April::TriangleFan,mVertices,4,r,g,b,a);
		else
			rendersys->render(April::TriangleFan,mVertices,4);
		if (mBlendMode != April::ALPHA_BLEND) rendersys->setBlendMode(April::DEFAULT);
		rendersys->popTransform();
	}

	void Image::draw(float centerx,float centery,float dw,float dh,float angle)
	{
		draw(centerx,centerx,dw,dh,angle,1,1,1,1);
	}

	April::Texture* Image::getTexture()
	{
		return mTexture;
	}


	ColoredImage::ColoredImage(April::Texture* tex,std::string name,float sx,float sy,float sw,float sh,bool vertical,unsigned int color) :
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


	ColorImage::ColorImage(std::string name) : Image(0,name,0,0,0,0,0)
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
		v[0].x=dx;    v[0].y=dy;	v[1].x=dx+dw; v[1].y=dy;
		v[2].x=dx+dw; v[2].y=dy+dh;	v[3].x=dx;    v[3].y=dy+dh;
		if (mBlendMode != April::ALPHA_BLEND) rendersys->setBlendMode(mBlendMode);
		rendersys->render(April::TriangleFan,v,4,mRed*r,mGreen*g,mBlue*b,mAlpha*a);
		if (mBlendMode != April::ALPHA_BLEND) rendersys->setBlendMode(April::DEFAULT);
	}

	void ColorImage::draw(float centerx,float centery,float dw,float dh,float angle,float r,float g,float b,float a)
	{
		April::PlainVertex v[4];
		v[0].x=-dw/2; v[0].y=-dh/2;	v[1].x= dw/2; v[1].y=-dh/2;
		v[2].x= dw/2; v[2].y= dh/2;	v[3].x=-dw/2; v[3].y= dh/2;

		
		rendersys->pushTransform();
		rendersys->setIdentityTransform();
		rendersys->translate(centerx,centery);
		rendersys->rotate(angle);
		if (mBlendMode != April::ALPHA_BLEND) rendersys->setBlendMode(mBlendMode);
		rendersys->render(April::TriangleFan,v,4,mRed*r,mGreen*g,mBlue*b,mAlpha*a);
		if (mBlendMode != April::ALPHA_BLEND) rendersys->setBlendMode(April::DEFAULT);
		rendersys->popTransform();
	}

	NullImage::NullImage() : Image(0,"null",0,0,0,0,0)
	{
		
	}
}
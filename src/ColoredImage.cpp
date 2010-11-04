/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <april/RenderSystem.h>

#include "ColoredImage.h"

namespace AprilUI
{
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

}

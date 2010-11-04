/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <gtypes/Matrix4.h>
#include <april/RenderSystem.h>

#include "ColorImage.h"
#include "Util.h"

namespace AprilUI
{
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

}

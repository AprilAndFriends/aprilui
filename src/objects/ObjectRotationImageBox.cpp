/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <hltypes/hstring.h>
#include <hltypes/util.h>

#include "Dataset.h"
#include "Image.h"
#include "ObjectRotationImageBox.h"

namespace AprilUI
{
	RotationImageBox::RotationImageBox(chstr name,float x,float y,float w,float h) :
		ImageBox(name,x,y,w,h)
	{
		mAngle=0;
	}

	void RotationImageBox::setProperty(chstr name,chstr value)
	{
		ImageBox::setProperty(name,value);
		if (name == "angle") mAngle=value;
	}

	void RotationImageBox::OnDraw(float offset_x,float offset_y)
	{
		if (!mImage) mImage=mDataPtr->getImage("null");
		float alpha=getDerivedAlpha();
		if (!getDerivedEnabled()) alpha/=2;
		mImage->draw(mX+offset_x+mWidth/2,mY+offset_y+mHeight/2,mWidth,mHeight,mAngle,1,1,1,alpha);
	}

	bool RotationImageBox::angleEquals(float angle)
	{
		float s1=dsin(angle),s2=dsin(mAngle);
		float c1=dcos(angle),c2=dcos(mAngle);
		return (fabs(s1-s2) < 0.01f && fabs(c1-c2) < 0.01f);
	}
	
}

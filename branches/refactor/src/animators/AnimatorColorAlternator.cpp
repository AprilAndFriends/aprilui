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

#include <hltypes/hstring.h>

#include "AnimatorColorAlternator.h"
#include "Image.h"
#include "Util.h"
#include "ObjectColoredImageBox.h"

namespace AprilUI
{
	namespace Animators
	{
		ColorAlternator::ColorAlternator(chstr name) : Animator("Animators::ColorAlternator",name,0,0,1,1)
		{
			mLow[0]=mLow[1]=mLow[2]=mLow[3]=0;
			mHigh[0]=mHigh[1]=mHigh[2]=mHigh[3]=1;
			mTimer=0;
			mSpeed=2;
		}

		void ColorAlternator::setProperty(chstr name,chstr value)
		{
			if (name == "low_color")       hexstr_to_argb_float(value,&mLow[0],&mLow[1],&mLow[2],&mLow[3]);
			else if (name == "high_color") hexstr_to_argb_float(value,&mHigh[0],&mHigh[1],&mHigh[2],&mHigh[3]);
			else if (name == "speed")      mSpeed=value;
		}

		void ColorAlternator::update(float k)
		{
			ColoredImage* img=0;
			ImageBox* parent=dynamic_cast<ImageBox*>(mParent);
			if (parent) img=dynamic_cast<ColoredImage*>(parent->getImage());
			if (!img) return;

			mTimer+=k*mSpeed;

			float a=(sin(mTimer)+1)*(mHigh[0]-mLow[0])/2+mLow[0];
			float r=(sin(mTimer)+1)*(mHigh[1]-mLow[1])/2+mLow[1];
			float g=(sin(mTimer)+1)*(mHigh[2]-mLow[2])/2+mLow[2];
			float b=(sin(mTimer)+1)*(mHigh[3]-mLow[3])/2+mLow[3];

			img->setColor(a,r,g,b);
		}
	}
}

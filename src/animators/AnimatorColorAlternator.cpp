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

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "AnimatorColorAlternator.h"
#include "ColoredImage.h"
#include "Util.h"
#include "ObjectColoredImageBox.h"

namespace aprilui
{
	namespace Animators
	{
		ColorAlternator::ColorAlternator(chstr name) : Animator("Animators::ColorAlternator", name, grect(0, 0, 1, 1))
		{
			mLow.set("00000000");
			mHigh.set("FFFFFFFF");
			mTimer = 0.0f;
			mSpeed = 2.0f;
		}

		bool ColorAlternator::isAnimated()
		{
			return true;
		}

		void ColorAlternator::setProperty(chstr name, chstr value)
		{
			if		(name == "low_color")	mLow.set(HEXCOLOR_FIX_FOR_NEW_APRIL(value));
			else if (name == "high_color")	mHigh.set(HEXCOLOR_FIX_FOR_NEW_APRIL(value));
			else if (name == "speed")		mSpeed = value;
		}

		void ColorAlternator::update(float k)
		{
			ColoredImage* image = NULL;
			ImageBox* parent = dynamic_cast<ImageBox*>(mParent);
			if (parent != NULL)
			{
				image = dynamic_cast<ColoredImage*>(parent->getImage());
			}
			if (image != NULL)
			{
				mTimer += k * mSpeed;
				image->setColor(((mHigh - mLow) / 2.f) * float(sin(mTimer) + 1) + mLow);
			}
		}
		
	}
}

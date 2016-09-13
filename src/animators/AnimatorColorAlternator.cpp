/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <math.h>

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "AnimatorColorAlternator.h"
#include "apriluiUtil.h"
#include "ColoredImage.h"
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

		bool ColorAlternator::setProperty(chstr name, chstr value)
		{
			if		(name == "low_color")	mLow.set(value);
			else if (name == "high_color")	mHigh.set(value);
			else if (name == "speed")		mSpeed = value;
			return 1;
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
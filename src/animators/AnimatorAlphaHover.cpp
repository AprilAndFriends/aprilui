/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <gtypes/Rectangle.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "Animators.h"

namespace aprilui
{
	namespace Animators
	{
		AlphaHover::AlphaHover(chstr name) : Animator("Animators::AlphaHover", name, grectf(0, 0, 1, 1))
		{
			mMin = 0.0f;
			mOwnerAlpha = 0.0f;
			mMax = 1.0f;
			mSpeed = 1.0f;
		}

		bool AlphaHover::isAnimated()
		{
			return true;
		}

		bool AlphaHover::setProperty(chstr name, chstr value)
		{
			if      (name == "min")		mMin = mOwnerAlpha = value;
			else if (name == "max")		mMax = value;
			else if (name == "speed")	mSpeed = value;
			return 1;
		}	

		void AlphaHover::notifyEvent(chstr name, void* params)
		{
			if (name == "AttachToObject")
			{
				mParent->setAlpha(mOwnerAlpha);
			}
			Object::notifyEvent(name, params);
		}

		void AlphaHover::update(float k)
		{
			int direction = (mParent->isCursorInside() ? 1 : -1);
			mOwnerAlpha += mSpeed * k * direction;
			mOwnerAlpha = hclamp(mOwnerAlpha, mMin, mMax);
			mParent->setAlpha(mOwnerAlpha);
		}
		
	}
}

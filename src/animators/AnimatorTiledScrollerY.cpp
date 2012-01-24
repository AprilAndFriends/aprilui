/// @file
/// @author  Boris Mikic
/// @version 1.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <gtypes/Rectangle.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "AnimatorTiledScrollerY.h"
#include "ObjectImageBox.h"
#include "TiledImage.h"

namespace aprilui
{
	namespace Animators
	{
		TiledScrollerY::TiledScrollerY(chstr name) : Animator(name)
		{
		}

		TiledScrollerY::~TiledScrollerY()
		{
		}

		Animator* TiledScrollerY::createInstance(chstr name)
		{
			return new TiledScrollerY(name);
		}

		void TiledScrollerY::notifyEvent(chstr name, void* params)
		{
			if (name == "AttachToObject" || name == "OnDelayEnd" && mInheritValue)
			{
				mValue = mOffset = mParent->getY();
				if (mUseTarget)
				{
					mAmplitude = mTarget - mValue;
				}
			}
			Object::notifyEvent(name, params);
		}
		
		void TiledScrollerY::update(float k)
		{
			bool animated = this->isAnimated();
			Animator::update(k);
			if (!animated)
			{
				return;
			}
			ImageBox* imageBox = dynamic_cast<ImageBox*>(mParent);
			if (imageBox == NULL)
			{
				aprilui::log("Animators::TiledScrollerY: parent object not a subclass of Objects::ImageBox!");
				return;
			}
			TiledImage* image = dynamic_cast<TiledImage*>(imageBox->getImage());
			if (image == NULL)
			{
				aprilui::log("Animators::TiledScrollerY: image in object not a subclass of Animators::TiledImage!");
				return;
			}
			mValue = image->getScrollY();
			mValue = _calculateValue(mTimeSinceLastFrame);
			image->setScrollY(mValue);
		}
		
	}
}

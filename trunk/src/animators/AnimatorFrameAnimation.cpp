/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>
#include <hltypes/util.h>

#include "AnimatorFrameAnimation.h"
#include "aprilui.h"
#include "Image.h"
#include "ObjectImageBox.h"

namespace aprilui
{
	namespace Animators
	{
		FrameAnimation::FrameAnimation(chstr name) : Animator(name)
		{
			mImageBaseName = "";
			mFirstFrame = 0;
			mFrameCount = 0;
		}

		FrameAnimation::~FrameAnimation()
		{
		}

		Animator* FrameAnimation::createInstance(chstr name)
		{
			return new FrameAnimation(name);
		}

		bool FrameAnimation::isAnimated()
		{
			if (!Animator::isAnimated())
			{
				return false;
			}
			if (mImageBaseName == "")
			{
				return false;
			}
			if (mFrameCount <= 0)
			{
				return false;
			}
			return true;
		}

		bool FrameAnimation::setProperty(chstr name, chstr value)
		{
			if		(name == "base_name")		mImageBaseName = value;
			else if (name == "first_frame")		mFirstFrame = value;
			else if (name == "frame_count")		mFrameCount = value;
			else if (name == "inherit_value")
			{
				aprilui::log("Warning: Animators::FrameAnimation does not support inherit_value");
			}
            else Animator::setProperty(name, value);
            return 1;
		}

		void FrameAnimation::notifyEvent(chstr name, void* params)
		{
			if (name == "AttachToObject")
			{
				mValue = (float)mFirstFrame;
				float delay = mDelay;
				mDelay = 0.0f;
				update(0.0f);
				mDelay = delay;
			}
			Animator::notifyEvent(name, params);
		}

		void FrameAnimation::update(float k)
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
				aprilui::log("Animators::FrameAnimation: parent object not a subclass of Objects::ImageBox!");
				return;
			}
			mValue = _calculateValue(mTimeSinceLastFrame);
			int frame = (int)mValue;
			int lastFrame = mFirstFrame + mFrameCount - 1;
			if (isExpired() || frame > lastFrame)
			{
				frame = (mReset ? mFirstFrame : lastFrame);
			}
			imageBox->setImageByName(mImageBaseName + hstr(frame));
		}
		
	}
}

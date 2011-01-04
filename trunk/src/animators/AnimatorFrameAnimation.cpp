/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
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
		FrameAnimation::FrameAnimation(chstr name) : Animator("Animators::FrameAnimation", name, grect(0, 0, 1, 1))
		{
			mImageBaseName = "";
			mFirstFrame = 0;
			mFrameCount = 0;
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

		void FrameAnimation::setProperty(chstr name, chstr value)
		{
			Animator::setProperty(name, value);
			if		(name == "base_name")	mImageBaseName = value;
			else if (name == "first_frame")	mFirstFrame = value;
			else if (name == "frame_count")	mFrameCount = value;
		}

		void FrameAnimation::notifyEvent(chstr name, void* params)
		{
			if (name == "AttachToObject")
			{
				mValue = mFirstFrame;
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
			mValue = _calculateValue(k);
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
